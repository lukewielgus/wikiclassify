#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
using std::cin;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::getline;
using std::size_t;
using std::to_string;

#include <time.h>

#include <ctime>

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

//Check article for cleanup templates
vector<string> getTemplates(string &input){
	vector<string> found;
	vector<string> templates = {"stub}}", "{{Cleanup", "{{Advert", "{{Update", "{{Tone", "{{Plot", "{{Essay-like", "{{Peacock", "{{Technical", "{{Confusing"};
	vector<string> clean = {"stub", "cleanup", "advert", "update", "tone", "plot", "essay-like", "peacock", "technical", "confusing"};
	for(int i=0; i<templates.size(); i++){
		if(input.find(templates[i]) != string::npos){
			found.push_back(clean[i]);
		}
	}
	return found;
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
	vector<string> templates;	 // Cleanup templates (if any)
	
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
	if(isWithin(text, "{{Featured article}}")){
		quality=2;
	}
	if(isWithin(text, "{{Good article}}")){
		quality=1;
	}
	//Get cleanup templates (if any)
	//templates = getTemplates(text);
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

//Remove all formatting artifacts
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
	file<<"---> VERSION 1.0\n";
	file<<(*this);
	file<<"\n"<<text<<"\n";
	file<<"---> EOA\n";
}

ostream& operator<<(ostream& os, wikiPage& wp)
{
    os <<"Title:\t\t"<<wp.title<<"\nNamespace:\t"<<wp.ns<<"\nArticle size:\t"<<wp.text.size()<<"\nRedirect:\t"<<wp.isRedirect<<"\nQuality:\t"<<wp.quality<<"\nContributor:\t"<<wp.contrib<<"\nTimestamp:\t"<<wp.timestamp<<"\nPic Count:\t"<<wp.pic<<"\n";
    return os;
}

void getPage(ifstream &dataDump, bool &end, string &pagestr){
	unsigned short buffersize = 4096;
	unsigned blocksize = 1000000;
	string block; 
	char buffer[buffersize];
	
	string line;
	bool condition=true;
	while(condition){
		getline(dataDump, line);
		if(line.find("<page>") != string::npos){
			while(true){
				getline(dataDump, line);
				if(line.find("</page>") != string::npos){
					return;
				}
				pagestr.append(line);
			}
		}
	}
}

void savePage(wikiPage &temp, ofstream &hash, unsigned long &featuredCt, unsigned long &goodCt, unsigned long &stubCt){
	string featFolder = "parsed/featured/vol-";
	string goodFolder = "parsed/good/vol-";
	string stubFolder = "parsed/stub/vol-";
	string file;
	switch (temp.quality){
		case 0:
		hash<<"["<<temp.title<<"] stub/vol-"<<stubCt<<".txt\n";
		file = stubFolder+to_string(stubCt)+".txt";
		stubCt++;
		break;
		case 1:
		hash<<"["<<temp.title<<"] good/vol-"<<goodCt<<".txt\n";
		file = goodFolder+to_string(goodCt)+".txt";
		goodCt++;
		break;
		case 2:
		hash<<"["<<temp.title<<"] featured/vol-"<<featuredCt<<".txt\n";
		file = featFolder+to_string(featuredCt)+".txt";
		featuredCt++;
	}
	ofstream output(file);
	temp.save(output);
	//output<<"SAVE DATE: "<<std::asctime(std::localtime(std::time(nullptr)))<<"\n";
	return;
}

void compile(string filename){
	string folder = "parsed/";
	string hashfile = "hashfile.txt";
	string filePre = "vol-";
	string fileEx = ".txt";

	unsigned long featuredCt = 0;
	unsigned long goodCt = 0;
	unsigned long stubCt = 0;

	ofstream hash(folder+hashfile);
	ifstream dataDump(filename);

	//hash<<"SAVE DATE: "<<std::asctime(std::localtime(std::time(nullptr)))<<"\n";

	unsigned long long pageCt = 0;
	float pageCtFloat = 0;
	bool end = false;

	cout<<"Fetching, parsing, and saving...\n";
	time_t start = clock();
	//cout<<"\rFeatured: "<<featuredCt<<"\t Good: "<<goodCt<<"\t Stub: "<<stubCt<<"\t Total: "<<pageCt<<"\t Progress: ~"<<(pageCtFloat/5000000)*100<<"%\t\t Articles/Second: "<<(pageCtFloat/((clock()-start)/CLOCKS_PER_SEC));
	while(dataDump.eof()==false){
		string pagestr;
		getPage(dataDump, end, pagestr);
		wikiPage temp(pagestr);
		savePage(temp, hash, featuredCt, goodCt, stubCt);
		pageCt++;
		pageCtFloat = pageCt;
		cout<<"\rFeatured: "<<featuredCt<<"\t Good: "<<goodCt<<"\t Stub: "<<stubCt<<"\t Total: "<<pageCt<<"\t Progress: ~"<<(pageCtFloat/5000000)*100<<"%\t Articles/Second: "<<(pageCtFloat/((clock()-start)/CLOCKS_PER_SEC));
		cout.flush();
	}
}
void setup(string &filename){
	cout<<"\nSkip setup? [Y/n]: ";
	string input;
	cin>>input;
	if(input=="Y" or input=="y"){
		cout<<"--> Starting process...\n\n";
		return;
	}
	cout<<"\n--> Is this the name of your data dump file: "<<filename<<" [Y/n]: ";
	cin>>input;
	if(input=="n" or input=="N"){
		cout<<"--> Enter the name of your data dump file: ";
		cin>>filename;
	}
	cout<<"--> The parsed wikiPages will be held in a sub-directory named parsed, have you created this? [Y/n]: ";
	cin>>input;
	if(input=="N" or input=="n"){
		cout<<"--> Create the folder and input 'ready' when completed: ";
		cin>>input;
	}
	cout<<"--> Within /parsed you need the following 3 folders; featured, good, & stub, do you have these? [Y/n]: ";
	cin>>input;
	if(input=="n" or input=="N"){
		cout<<"--> Create these three folders, input 'ready' when complete: ";
		cin>>input;
	}
	cout<<"--> Starting process... \n\n";
}

int main(){
	//get_max_fpos("enwiki-20160113-pages-articles.xml");
	string filename = "enwiki-20160113-pages-articles.xml";
	setup(filename);
	compile(filename);
}






