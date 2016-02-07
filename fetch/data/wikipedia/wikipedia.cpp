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

#include <algorithm>

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

//Grab the number of pictures in article
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
	float times;
private:
	time_t tclock;
};

void timeit::start() {
	tclock = clock();
}

void timeit::stop() {
	times = (float(clock()-tclock)/CLOCKS_PER_SEC);
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

	//Convert entire text to lower case
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
}


//Both begintarg & endtarg must be same length for function to work
void nestedRemoval(string begintarg, string endtarg, string &text, size_t &current, size_t begin, int &openCt){
	if(openCt==0){
		text.erase(begin, current+endtarg.size()-begin);
		return;
	}
	size_t closeLocation = text.find(endtarg, current+begintarg.size());
	if(closeLocation==string::npos){
		text.erase(begin, string::npos);
		return;
	}
	size_t temp = current;
	while(true){
		size_t nests = text.find(begintarg, temp+begintarg.size());
		if((nests>closeLocation) or (nests==string::npos)){
			break;
		}
		else{
			openCt++;
			temp = nests;
		}
	}
	openCt--;
	return nestedRemoval(begintarg, endtarg, text, closeLocation, begin, openCt);
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
			size_t endlocation = temp.find(endtarget, location+target.size());
			temp.erase(location, endlocation+endtarget.size()-location);
		}
		else{
			condition=false;
		}
	}

	//Removing all content between curly brackets
	condition=true;
	string open = "{{";
	string close = "}}";
	while(condition){
		size_t location = temp.find(open);
		if(location!=string::npos){
			int openCt=1;
			nestedRemoval(open, close, temp, location, location, openCt);
		}
		else{
			condition=false;
		}
	}
	//Removing all content between brackets
	condition=true;
	open = "[[";
	close = "]]";
	while(condition){
		size_t location = temp.find(open);
		if(location!=string::npos){
			int openCt=1;
			nestedRemoval(open, close, temp, location, location, openCt);
		}
		else{
			condition=false;
		}
	}
	//Removing all sub-category headers
	condition=true;
	string both = "===";
	while(condition){
		size_t location = temp.find(both);
		if(location!=string::npos){
			size_t end = temp.find(both, location+both.size());
			temp.erase(location, end+both.size()-location);
		}
		else{
			condition=false;
		}
	}
	//Removing all category headers
	condition=true;
	both = "==";
	while(condition){
		size_t location = temp.find(both);
		if(location!=string::npos){
			size_t end = temp.find(both, location+both.size());
			temp.erase(location, end+both.size()-location);
		}
		else{
			condition=false;
		}
	}
	//Adding junk formatting to the targets vector...
	vector<string> targets{"'''","&lt;","&quot;","''","*","[","]","&gt;","ref",".",",","!",":","?",";","(",")","$","'","&","ampndash"};
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
	//Characters to be replaced by a space
	vector<string> tobereplaced{"-","|","/","="};
	for(int i=0; i<tobereplaced.size(); i++){
		target = tobereplaced[i];
		condition=true;
		while(condition){
			size_t location = temp.find(target);
			if(location!=string::npos){
				temp.replace(location, 1, " ");
			}
			else{
				condition=false;
			}
		}
	}
	//Remove empty lines
	string twoline = "\n\n";
	string oneline = "\n";
	condition = true;
	while(condition){
		size_t location = temp.find(twoline);
		if(location!=string::npos){
			temp.replace(location, twoline.size(), oneline);
		}
		else{
			condition=false;
		}
	}
	text = temp;
	return;
}

//Save function (save to file)
void wikiPage::save(ofstream &file){
	file<<"---> SAVE VERSION 1.0\n";
	file<<(*this);
	file<<text<<"\n";
	file<<"---> END OF ARTICLE\n";
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

void backup(unsigned long long &fpos, string filename){
	
	ofstream file(filename);
	file<<fpos;
	cout<<"File position data cached to: "<<filename<<"\n";
}

vector<string> getPages(int numpages, ifstream &dataDump, unsigned long long &fpos, bool &done) {
	
	unsigned short buffersize = 4096;               // Number of characters buffered at a time
	unsigned blocksize = 1000000;                   // Size of string searched at a time; larger than largest article (apprx 800000)
	string block; char buffer[buffersize];          // String objects for buffering
	
	vector<string> pages;                           // Initialize return value
	while(pages.size()<numpages) {                  // While pages is below the specified size
		// Load buffer into string
		block = "";                                 // New string being searched
		dataDump.seekg(fpos);                       // Seek to current file position (position after last match)
		while (block.size() < blocksize) {          // While string size is less than the block size
			dataDump.read(buffer, sizeof(buffer));
			if(dataDump.eof()){
				done = true;
				return pages;
			}
			block.append(buffer, sizeof(buffer));
		}
		fpos += parse(block, "<page>", "</page>", pages);
	}
	return pages;
}

void flush(vector<wikiPage> &pages, int &numDone, ofstream &titleTable, timeit &timer){
	//Saving the pages...
	int num_articles = pages.size();


	string saveFilePre = "Parsed_WikiPages/vol-";
	string saveFileExt = ".txt";
	titleTable<<"\n--> vol-"<<numDone<<".txt "<<num_articles<<" articles ...\n";
	string filename = saveFilePre+std::to_string(numDone)+saveFileExt;

	ofstream file(filename);

	for(int i=0; i<pages.size(); i++){
		pages[i].save(file);
		titleTable<<pages[i].title<<"\n";
	}

	timer.stop();
	cout<<"Saved "<<num_articles<<" articles to "<<filename<<" in "<<timer.times<<" seconds                          \n";
	timer.start();
	numDone++;
	vector<wikiPage> newPage;
	pages=newPage;
}


void fetch_and_save(int numpages, int articlesPerPage, ifstream &dataDump, int swap, unsigned long long &fpos, int &fileCt, ofstream &titleTable, bool &done){
	timeit timer;
	cout<<"Fetching...\n";
	unsigned long long prior_fpos = fpos;
	vector<string> raw_pages = getPages(numpages, dataDump, fpos, done);
	backup(fpos,"Parsed_WikiPages/fpos_cached.txt");
	backup(prior_fpos,"Parsed_WikiPages/prior_fpos_cached.txt");
	float counter=0;
	vector<wikiPage> pages;
	timer.start();
	for(string i : raw_pages){
		counter++;
		float percent = (counter/raw_pages.size())*100;
		int percentInt = percent;
		cout<<"\r"<<percentInt<<"% Complete ("<<swap<<"/102)\t[";
		for(int j=0; j<50; j++){
			if(percent/2>j){
				cout<<"|";
			}
			else{
				cout<<" ";
			}
		}
		cout<<"]";
		cout.flush();
		wikiPage x(i);
		if(x.ns == "0" and not x.isRedirect){
			pages.push_back(x);
			if(pages.size()>=articlesPerPage){
				cout<<"\r";
				flush(pages, fileCt, titleTable, timer);
			}
		}
	}
	cout<<"\r";
	flush(pages, fileCt, titleTable, timer);
	cout<<"\n";
}

void run(){
	//About 1GB per 50000 articles
	int articles_per_swap = 50000;
	int articles_per_page = 5000;
	long total_articles = 5073000;

	//Initializing ifstream
	string filename = "enwiki-20160113-pages-articles.xml";
	ifstream dataDump(filename);
	unsigned long long fpos=0;

	int fileCt=0;
	string tablefile = "Parsed_WikiPages/titleTable.txt";
	ofstream titleTable(tablefile);
	bool done = false;
	for(int i=0; i<102; i++){
		fetch_and_save(articles_per_swap, articles_per_page, dataDump, i, fpos, fileCt, titleTable, done);
		if(done){
			cout<<"Done... See titleTable.txt for seaching\n";
			return;
		}
	}
	cout<<"Done... See titleTable.txt for seaching\n";
}

unsigned long long get_max_fpos(string filename){
	ifstream file(filename);
	unsigned long long fpos=0;
	cout<<"Finding the max fpos... ["<<fpos<<"]";
	char buffer[5000];
	while(true){
		file.seekg(fpos);
		file.read(buffer, sizeof(buffer));
		if(file.eof()){
			cout<<"Found max fpos: "<<fpos<<"\n";
			return fpos;
		}
		cout<<"\rFinding the max fpos... ["<<fpos<<"]";
		cout.flush();
		fpos+=5000;
	}
}


int main(){
	//get_max_fpos("enwiki-20160113-pages-articles.xml");
	run();
}





