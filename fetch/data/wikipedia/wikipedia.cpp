#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::getline;
using std::size_t;

#include <time.h>

//Check if string "tag1" is within string "str"
bool isWithin(string &str, string tag1) {
	return (str.find(tag1) != string::npos);
}

// Parse the contents between a pair of tags
string parse(string &str, string tag1, string tag2) {
	size_t p1 = str.find(tag1);
	size_t p2 = str.find(tag2, p1);
  	if (p1 != string::npos and p2 != string::npos) {
  		return str.substr(p1+tag1.size(), p2-p1-tag1.size());
  	}
  	else {
  		return "";
  	}
}

// Populate the given vector with all matches, in given string, between given tags
// Returns the index immediately after last match (for purposes of block shifting)
int parse(string &str, string tag1, string tag2, vector<string> &result) {
	int pos = 0;                                            // Current position in string
	while(true) {
		size_t p1 = str.find(tag1, pos);
		size_t p2 = str.find(tag2, p1);
  		if (p1 != string::npos and p2 != string::npos) {    // If new match, push it back
  			pos = p2+tag2.size();
  			string parsed = str.substr(p1+tag1.size(), p2-p1-tag1.size());
  			result.push_back(parsed);
  		}
		else {break;}                                       // Break otherwise
	}
	return pos;
}

short picCount(string &article){
	short count=0;
	bool condition=true;
	string pic = ".jpg";
	size_t location = 0;
	while(condition){
		location = article.find(pic, location);
		if(location!=string::npos){
			count++;
			location += pic.size();
		}
		else{
			condition=false;
		}
	}
	return count;
}

// Small timer class
class timeit {
public:
	void start();
	void stop();
	vector<float> times;
private:
	time_t tclock;
};

void timeit::start() {
	tclock = clock();
}

void timeit::stop() {
	times.push_back(float(clock()-tclock)/CLOCKS_PER_SEC);
}

class wikiPage {
public:
	string         title;        // Page title
	string         ns;           // Page namespace
	string         text;         // Page wikimarkup
	vector<string> categories;   // Page categories
	bool           isRedirect;   // Page redirect status
	int            quality;      // Page quality (0: stub, 1: other, 2: good/featured)
	string         contrib;      // Revision contributor
	string         timestamp;    // Revision timestamp
	short          pic;          // Total picture count
	
	wikiPage(string pagestr);    // Constructor
	void save(ofstream &file);
	void removeJunk();
	friend ostream& operator<<(ostream& os, wikiPage& wp);
};

// wikipage constructor
wikiPage::wikiPage(string pagestr) {
	//Set the title of the page
	title = parse(pagestr, "<title>", "</title>");
	//Set the namespace
	ns = parse(pagestr, "<ns>", "</ns>");
	//Grab the text portion of the page
	text = parse(pagestr, "<text xml:space=\"preserve\">", "</text>");

	//Grab the categories for the page
	parse(pagestr, "[[Category:", "]]", categories);
	//Grab the contributor for latest edit
	contrib = parse(pagestr, "<username>", "</username>");
	//Grab the latest timestamp on the page
	timestamp = parse(pagestr, "<timestamp>", "</timestamp>");
	
	// Categorize based on text
	isRedirect = isWithin(text, "#REDIRECT");
	quality = 0;
	//Set the quality if a featured or good article
	if (isWithin(text, "{{Featured article}}") || isWithin(text, "{{Good article}}")) {
		quality = 1;
	}

	//Count number of pictures present in article
	pic = picCount(pagestr);

	//Remove Junk from text
	removeJunk();
}

void wikiPage::removeJunk() {
	string temp = text;
	//Searching for categories not yet removed
	bool condition=true;
	string target = "[[Category:";
	string endtarget = "]]";
	while(condition){
		size_t location = temp.find(target);
		if(location!=string::npos){
			size_t endlocation = temp.find(endtarget, target.size());
			temp.erase(location, endlocation+endtarget.size());
		}
		else{
			condition=false;
		}
	}
	
	condition=true;
	target = "{{cite";
	endtarget = "}}";
	while(condition){
		size_t location = temp.find(target);
		if(location!=string::npos){
			size_t endlocation = temp.find(endtarget, target.size());
			temp.erase(location, endlocation+endtarget.size());
		}
		else{
			condition=false;
		}
	}
	/*
	//Adding junk formatting to the targets vector...
	vector<string> targets{"'''","&lt;","&gt;","&quot;","''","[[","]]"};
	//Removing all instances of junk strings...
	for(int i=0; i<targets.size(); i++){
		target = targets[i];
		condition=true;
		while(condition){
			size_t location = temp.find(target);
			if(location!=string::npos){
				temp.erase(location,target.size());
			}
			else{
				condition=false;
			}
		}
	}
	*/
	text = temp;
	return;
}

//Save function (save to file)
void wikiPage::save(ofstream &file){
	file<<"-------------> SAVE VERSION 1.0 <-------------\n";
	file<<(*this);
	file<<text<<"\n";
	file<<"-----------------------------------------------\n";
}

ostream& operator<<(ostream& os, wikiPage& wp)
{
    os <<"Title:\t\t"<<wp.title<<"\nNamespace:\t"<<wp.ns<<"\nArticle size:\t"<<wp.text.size()<<"\nRedirect:\t"<<wp.isRedirect<<"\nQuality:\t"<<wp.quality<<"\nContributor:\t"<<wp.contrib<<"\nTimestamp:\t"<<wp.timestamp<<"\nPic Count:\t"<<wp.pic<<"\n";
    return os;
}

vector<string> getPages(string &filename, int numpages) {
	/* Gets a vector of page strings */

	ifstream dataDump(filename);                    // Create filestream
	
	unsigned short buffersize = 4096;               // Number of characters buffered at a time
	unsigned blocksize = 1000000;                   // Size of string searched at a time; larger than largest article (apprx 800000)
	string block; char buffer[buffersize];          // String objects for buffering
	
	vector<string> pages;                           // Initialize return value
	unsigned long long fpos = 0;                    // Position in file
	while(pages.size() < numpages) {                // While pages is below the specified size
		// Load buffer into string
		block = "";                                 // New string being searched
		dataDump.seekg(fpos);                       // Seek to current file position (position after last match)
		while (block.size() < blocksize) {          // While string size is less than the block size
			dataDump.read(buffer, sizeof(buffer));
			block.append(buffer, sizeof(buffer));
		}
	
		fpos += parse(block, "<page>", "</page>", pages);
		
	}
	return pages;
}

int main(){
	string filename = "enwiki-20160113-pages-articles.xml";
	vector<string> raw_pages = getPages(filename, 100);
	
	vector<wikiPage> pages;
	for(string i : raw_pages){
		wikiPage x(i);
		if(x.ns == "0" and not x.isRedirect){
			pages.push_back(x);
		}
	}

	string saveFile = "test.txt";
	ofstream file(saveFile);
	pages[10].save(file);
}

/*
int main(int argc, char** argv) {

	string filename = "enwiki-20160113-pages-articles.xml";
	
	// Get vector of raw page strings
	cout<<"Getting raw page strings..."<<endl;
	
	// WARNING -- RAM size requirements is about 1 GB per 30,000 articles
	int npages = 30000;

	//Start timer for raw page grab
	timeit timer;
	
	timer.start();
	vector<string> raw_pages = getPages(filename, npages);
	timer.stop();
	
	// Vector of wikipage objects
	vector<wikiPage> pages;
	
	// Populate pages with initialized wikiPages
	timer.start();
	cout<<"Parsing raw page strings..."<<endl;
	for (string i : raw_pages) {
		wikiPage x(i);
		if (x.ns == "0" and not x.isRedirect) {
			pages.push_back(x);
		}
	}
	timer.stop();
	
	cout<<"Parsing time: "<<timer.times[0]<<" Seconds per Page\n";
	cout<<"wikiPage time: "<<timer.times[1]<<" Seconds per Page\n";
	
	return 0;
}
*/