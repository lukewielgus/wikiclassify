#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::getline;
using std::size_t;

// Parse the contents between a pair of tags
string parseTag(string &str, string tag1, string tag2) {
	size_t p1 = str.find(tag1);
	size_t p2 = str.find(tag2, p1);
  	if (p1 != string::npos and p2 != string::npos) {
  		return str.substr(p1+tag1.size(), p2-p1-tag2.size()+1);
  	}
  	else {
  		return "";
  	}
}

// Populate the given vector with all matches, in given string, between given tags
// Returns the index immediately after last match (for purposes of block shifting)
int parseTags(vector<string> &result, string &str, string tag1, string tag2) {
	int pos = 0;                                            // Current position in string
	while(true) {
		size_t p1 = str.find(tag1, pos);
		size_t p2 = str.find(tag2, p1);
  		if (p1 != string::npos and p2 != string::npos) {    // If new match, push it back
  			pos = p2+tag2.size();
  			string parse = str.substr(p1+tag1.size(), p2-p1-tag1.size());
  			result.push_back(parse);
  		}
		else {break;}                                       // Break otherwise
	}
	return pos;
}

class wikiPage {
public:
	string         title;        // Page title
	string          ns;          // Page namespace
	string         text;         // Page wikimarkup
	vector<string> categories;   // Page categories
	string         contrib;      // Revision contributor
	string         timestamp;    // Revision timestamp
	
	wikiPage(string pagestr);    // Constructor
};

// wikipage constructor
wikiPage::wikiPage(string pagestr) {
	title = parseTag(pagestr, "<title>", "</title>");
	ns = parseTag(pagestr, "<ns>", "</ns>");
	text = parseTag(pagestr, "<text xml:space=\"preserve\">", "</text>");
	parseTags(categories, pagestr, "[[Category:", "]]");
	contrib = parseTag(pagestr, "<username>", "</username>");
	timestamp = parseTag(pagestr, "<timestamp>", "</timestamp>");
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
	
		fpos += parseTags(pages, block, "<page>", "</page>");
		
	}
	return pages;
}

int main(int argc, char** argv) {

	// Dump filename
	string filename = "enwiki-20151201-pages-articles.xml";
	
	// Get vector of raw page strings
	cout<<"Getting raw page strings..."<<endl;
	vector<string> raw_pages = getPages(filename, 1000);
	
	// Vector of wikipage objects
	vector<wikiPage> pages;
	
	// Populate pages with initialized wikiPages
	cout<<"Parsing raw page strings..."<<endl;
	for (string i : raw_pages) {
		wikiPage x(i);
		if (x.ns == "0") {
			cout<<x.contrib<<endl;
			pages.push_back(x);
		}
	}
	
	cout<<pages.size()<<endl;
	return 0;
}