#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::getline;
using std::size_t;
using std::stoi;


//Add the timer function using time.h
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


class wikiPage {
public:
	string         title;        // Page title
	short          ns;           // Page namespace
	string         text;         // Page wikimarkup
	vector<string> categories;   // Page categories
	bool           isRedirect;   // Page redirect status
	int            quality;      // Page quality (0: stub, 1: other, 2: good/featured)
	string         contrib;      // Revision contributor
	string         timestamp;    // Revision timestamp
	short          pic;          // Total picture count
	
	wikiPage(string pagestr);    // Constructor
	friend ostream& operator<<(ostream& os, wikiPage& wp);
};

// wikipage constructor
wikiPage::wikiPage(string pagestr) {
	//Set the title of the page
	title = parse(pagestr, "<title>", "</title>");
	//Set the namespace
	ns = stoi(parse(pagestr, "<ns>", "</ns>"));
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
	pic = picCount(pagestr);
}

ostream& operator<<(ostream& os, wikiPage& wp)
{
    os <<"\nTitle:\t\t"<<wp.title<<"\nNamespace:\t"<<wp.ns<<"\nArticle size:\t"<<wp.text.size()<<"\nRedirect:\t"<<wp.isRedirect<<"\nQuality:\t"<<wp.quality<<"\nContributor:\t"<<wp.contrib<<"\nTimestamp:\t"<<wp.timestamp<<"\nPic Count:\t"<<wp.pic<<"\n";
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

void removeJunk(wikiPage &input){
	string temp = input.text;
	//Searching for triple apostrophe formatting
	bool condition=true;
	string tripleAp = "'''";
	while(condition){
		size_t location = temp.find(tripleAp);
		if(location!=string::npos){
			temp.erase(location, tripleAp.size());
		}
		else{
			condition=false;
		}
	}
	//Searching for &lt text
	condition=true;
	string target = "&lt";
	while(condition){
		size_t location = temp.find(target);
		if(location!=string::npos){
			temp.erase(location, target.size());
		}
		else{
			condition=false;
		}
	}
	input.text = temp;
	return;
}


int main(){
	string filename = "enwiki-20160113-pages-articles.xml";
	vector<string> raw_pages = getPages(filename, 100);
	vector<wikiPage> pages;
	for(int i=0; i<raw_pages.size(); i++){
		wikiPage x(raw_pages[i]);
		if(not x.ns and not x.isRedirect){
			pages.push_back(x);
		}
	}
	std::cout<<pages[10];
	std::cout<<pages[10].text<<"\n";
	removeJunk(pages[10]);
	std::cout<<pages[10].text<<"\n";
}

/*
int main(int argc, char** argv) {

	// Dump filename
	string filename = "enwiki-20151201-pages-articles.xml";
	string bfaurefilename = "enwiki-20160113-pages-articles.xml";
	
	// Get vector of raw page strings
	cout<<"Getting raw page strings..."<<endl;
	
	// WARNING -- RAM size requirements is about 1 GB per 30,000 articles
	int npages = 30000;

	//Start timer for raw page grab
	time_t beforeRaw = clock();
	vector<string> raw_pages = getPages(bfaurefilename, npages);
	//End timer for raw page grab
	time_t afterRaw = clock();
	//Calculate elapsed time
	double secondsForRaw = double(afterRaw-beforeRaw)/CLOCKS_PER_SEC;
	cout<<"Time for raw page grab: "<<secondsForRaw<<" Seconds\n";
	cout<<"                        "<<secondsForRaw/npages<<" Seconds per Page\n";
	
	// Vector of wikipage objects
	vector<wikiPage> pages;
	
	// Populate pages with initialized wikiPages
	time_t beforeParse = clock();
	cout<<"Parsing raw page strings..."<<endl;
	for (string i : raw_pages) {
		wikiPage x(i);
		if (not x.ns and not x.isRedirect) {
			pages.push_back(x);
		}
	}
	time_t afterParse = clock();
	double secondsForParse = double(afterParse-beforeParse)/CLOCKS_PER_SEC;
	cout<<"Time for parsing: "<<secondsForParse<<" Seconds\n";
	cout<<"                  "<<secondsForParse/npages<<" Seconds per Page\n";
	return 0;
}

*/














