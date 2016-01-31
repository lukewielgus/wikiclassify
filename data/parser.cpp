#include <iostream>
<<<<<<< HEAD
#include <fstream>
#include <vector>
#include <string>


class wikiPage{
public:
	std::string timeStamp;
	bool featuredArticle;
	bool goodArticle;
	std::vector<std::string> category;
	std::string title;
	std::vector<std::string> body;
};

bool isTitle(char temp[500000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='t'){
				if(temp[i+2]=='i'){
					if(temp[i+3]=='t'){
						if(temp[i+4]=='l'){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool isEnd(char temp[500000]){
	std::string newTemp = temp;
	std::string target = "</page>";
	std::size_t found = newTemp.find(target);
	if(found!=std::string::npos){
		return true;
	}
	else{
		return false;
	}
}

std::string fixTitle(char temp[500000]){
	int size = strlen(temp);
	unsigned long index=0;
	bool condition=true;
	while(condition){
		if(temp[index]=='<'){
			condition=false;
		}
		else{
			index++;
		}
	}
	char newTemp[1000];
	index = index+7;
	unsigned long newIndex=0;
	condition=true;
	while(condition){
		newTemp[newIndex]=temp[index];
		newIndex++;
		index++;
		if(temp[index]=='<'){
			newTemp[newIndex]='\0';
			condition=false;
		}
	}
	return newTemp;
}

bool isRedirect(char temp[500000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='r'){
				if(temp[i+2]=='e'){
					if(temp[i+3]=='d'){
						if(temp[i+4]=='i'){
							if(temp[i=5]=='r'){
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool isFeatured(char temp[500000]){
	std::string newTemp = temp;
	std::string target = "{{featured article}}";
	std::size_t found = newTemp.find(target);
	if(found!=std::string::npos){
		return true;
	}
	else{
		return false;
	}
}

bool isCategory(char temp[500000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='['){
			if(temp[i+1]=='['){
				if(temp[i+2]=='C'){
					if(temp[i+3]=='a'){
						if(temp[i+4]=='t'){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

std::string getCategory(char temp[500000]){
	int size = strlen(temp);
	bool condition = true;
	unsigned long index=0;
	while(condition){
		if(temp[index]=='['){
			condition=false;
		}
		else{
			index++;
		}
	}
	index = index+11;
	condition=true;
	char newTemp[1000];
	unsigned long newIndex=0;
	while(condition){
		newTemp[newIndex]=temp[index];
		newIndex++;
		index++;
		if(temp[index]==']'){
			condition=false;
		}
		if(temp[index]=='|'){
			condition=false;
		}
	}
	newTemp[newIndex]='\0';
	return newTemp;
}

bool isUseless(char temp[500000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='n'){
				if(temp[i+2]=='s'){
					return true;
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='f'){
				if(temp[i+2]=='o'){
					if(temp[i+3]=='r'){
						if(temp[i+4]=='m'){
							if(temp[i+5]=='a'){
								return true;
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='p'){
				if(temp[i+2]=='a'){
					if(temp[i+3]=='g'){
						if(temp[i+4]=='e'){
							if(temp[i+5]=='>'){
								return true;
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='/'){
				if(temp[i+2]=='p'){
					if(temp[i+3]=='a'){
						if(temp[i+4]=='g'){
							return true;
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='{'){
			if(temp[i+1]=='{'){
				if(temp[i+2]=='D'){
					if(temp[i+3]=='E'){
						if(temp[i+4]=='F'){
							if(temp[i+5]=='A'){
								if(temp[i+6]=='U'){
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='s'){
				if(temp[i+2]=='h'){
					if(temp[i+3]=='a'){
						if(temp[i+4]=='1'){
							return true;
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='r'){
				if(temp[i+2]=='e'){
					if(temp[i+3]=='v'){
						if(temp[i+4]=='i'){
							if(temp[i+5]=='s'){
								return true;
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='/'){
				if(temp[i+2]=='r'){
					if(temp[i+3]=='e'){
						if(temp[i+4]=='v'){
							if(temp[i+5]=='i'){
								if(temp[i+6]=='s'){
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='m'){
				if(temp[i+2]=='o'){
					if(temp[i+3]=='d'){
						if(temp[i+4]=='e'){
							if(temp[i+5]=='l'){
								if(temp[i+6]=='>'){
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='i'){
				if(temp[i+2]=='d'){
					if(temp[i+3]=='>'){
						return true;
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='p'){
				if(temp[i+2]=='a'){
					if(temp[i+3]=='r'){
						if(temp[i+4]=='e'){
							if(temp[i+5]=='n'){
								if(temp[i+6]=='t'){
									if(temp[i+7]=='i'){
										if(temp[i+8]=='d'){
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='c'){
				if(temp[i+2]=='o'){
					if(temp[i+3]=='n'){
						if(temp[i+4]=='t'){
							if(temp[i+5]=='r'){
								if(temp[i+6]=='i'){
									if(temp[i+7]=='b'){
										if(temp[i+8]=='u'){
											if(temp[i+9]=='t'){
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='c'){
				if(temp[i+2]=='o'){
					if(temp[i+3]=='m'){
						if(temp[i+4]=='m'){
							if(temp[i+5]=='e'){
								if(temp[i+6]=='n'){
									if(temp[i+7]=='t'){
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='/'){
				if(temp[i+2]=='c'){
					if(temp[i+3]=='o'){
						if(temp[i+4]=='n'){
							if(temp[i+5]=='t'){
								if(temp[i+6]=='r'){
									if(temp[i+7]=='i'){
										if(temp[i+8]=='b'){
											if(temp[i+9]=='u'){
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='u'){
				if(temp[i+2]=='s'){
					if(temp[i+3]=='e'){
						if(temp[i+4]=='r'){
							if(temp[i+5]=='n'){
								if(temp[i+6]=='a'){
									if(temp[i+7]=='m'){
										if(temp[i+8]=='e'){
											if(temp[i+9]=='>'){
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool isTimeStamp(char temp[500000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='<'){
			if(temp[i+1]=='t'){
				if(temp[i+2]=='i'){
					if(temp[i+3]=='m'){
						if(temp[i+4]=='e'){
							if(temp[i+5]=='s'){
								if(temp[i+6]=='t'){
									if(temp[i+7]=='a'){
										if(temp[i+8]=='m'){
											if(temp[i+9]=='p'){
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

std::string getTimeStamp(char temp[500000]){
	int size = strlen(temp);
	bool condition = true;
	unsigned long index=0;
	while(condition){
		if(temp[index]=='<'){
			condition=false;
		}
		else{
			index++;
		}
	}
	index = index+11;
	condition=true;
	char newTemp[1000];
	unsigned long newIndex=0;
	while(condition){
		newTemp[newIndex]=temp[index];
		newIndex++;
		index++;
		if(temp[index]=='Z'){
			newTemp[newIndex]='\0';
			condition=false;
		}
	}
	return newTemp;
}

bool isGood(char temp[500000]){
	std::string newTemp = temp;
	std::string target = "{{good article}}";
	std::size_t found = newTemp.find(target);
	if(found!=std::string::npos){
		return true;
	}
	else{
		return false;
	}
}

bool titleSearch(std::string filename, std::string target, wikiPage &inputPage){
	bool condition = true;
	std::ifstream dataDump(filename);
	char temp[500000];
	bool found = false;
	unsigned long counter=0;
	unsigned long counterTwo=0;
	while(condition){
		dataDump.getline(temp, 500000);
		if(dataDump.eof()){
			return false;
		}
		if(isTitle(temp)){
			counter++;
			std::cout<<"Article Number: "<<counter<<std::endl;
			if(!isRedirect(temp)){
				std::string title = fixTitle(temp);
				if(title==target){
					found = true;
					inputPage.title = title;
					char tempTwo[500000];
					while(condition){
						counterTwo++;
						std::cout<<"Article Line: "<<counterTwo<<std::endl;
						dataDump.getline(tempTwo, 500000);
						if(isTitle(tempTwo)){
							condition=false;
						}
						else{
							if(isFeatured(tempTwo)){
								inputPage.featuredArticle=true;
							}
							else{
								if(isCategory(tempTwo)){
									std::string cat = getCategory(tempTwo);
									inputPage.category.push_back(cat);
								}
								else{
									if(isTimeStamp(tempTwo)){
										std::string timeStamp = getTimeStamp(tempTwo);
										inputPage.timeStamp = timeStamp;
									}
									else{
										if(isGood(tempTwo)){
											inputPage.goodArticle=true;
										}
										else{
											inputPage.body.push_back(tempTwo);
										}
									}
								}
							}
						}
=======
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;
>>>>>>> nathankjer/master

#include <string>
using std::string;
using std::getline;
using std::size_t;
using std::stoi;

<<<<<<< HEAD
				}
			}
		}
	}
	if(found){
		std::cout<<"Searched "<<counter<<" articles\n";
		return true;
	}
	else{
		std::cout<<"Searched "<<counter<<" articles\n";
		return false;
	}
=======
bool isWithin(string &str, string tag1) {
	return (str.find(tag1) != string::npos);
>>>>>>> nathankjer/master
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

<<<<<<< HEAD


int main(){
	wikiPage page;
	std::cout<<"Enter the article you are looking for: ";
	std::string title;
	std::cin>>title;
	std::string filename = "enwiki-20160113-pages-articles.xml";
	std::cout<<"Searching...\n";
	if(titleSearch(filename, title, page)){
		std::cout<<"Found the article"<<std::endl;
		if(page.featuredArticle){
			std::cout<<"This article is featured!\n";
		}
		if(page.goodArticle){
			std::cout<<"This article is good!\n";
		}
		std::cout<<"-->[1] View the article"<<std::endl;
		std::cout<<"-->[2] View the timeStamp"<<std::endl;
		std::cout<<"-->[3] Exit"<<std::endl;
		int input;
		std::cin>>input;
		switch(input){
			case 1:
			for(int i=0; i<page.body.size(); i++){
				std::cout<<page.body[i]<<std::endl;
			}
			break;
			case 2:
			std::cout<<page.timeStamp<<std::endl;
			break;
			case 3:
			return 1;
		}
	}
	else{
		std::cout<<"Article not found"<<std::endl;
=======
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
>>>>>>> nathankjer/master
	}
	return pos;
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
	
	wikiPage(string pagestr);    // Constructor
};

// wikipage constructor
wikiPage::wikiPage(string pagestr) {
	title = parse(pagestr, "<title>", "</title>");
	ns = stoi(parse(pagestr, "<ns>", "</ns>"));
	text = parse(pagestr, "<text xml:space=\"preserve\">", "</text>");
	parse(pagestr, "[[Category:", "]]", categories);
	contrib = parse(pagestr, "<username>", "</username>");
	timestamp = parse(pagestr, "<timestamp>", "</timestamp>");
	
	// Categorize based on text
	isRedirect = isWithin(text, "#REDIRECT");
	quality = 0;
	if (isWithin(text, "{{Featured article}}") || isWithin(text, "{{Good article}}")) {
		quality = 1;
	}
	
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

int main(int argc, char** argv) {

	// Dump filename
	string filename = "enwiki-20151201-pages-articles.xml";
	
	// Get vector of raw page strings
	cout<<"Getting raw page strings..."<<endl;
	
	// WARNING -- RAM size requirements is about 1 GB per 30,000 articles
	int npages = 30000;
	vector<string> raw_pages = getPages(filename, npages);
	
	// Vector of wikipage objects
	vector<wikiPage> pages;
	
	// Populate pages with initialized wikiPages
	cout<<"Parsing raw page strings..."<<endl;
	for (string i : raw_pages) {
		wikiPage x(i);
		if (not x.ns and not x.isRedirect) {
			pages.push_back(x);
		}
	}
	
	return 0;
	
}