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
using std::size_t;
using std::getline;

#include <cmath>

bool checkFormat(string &input){
	bool check = false;
	string target = "\"";
	vector<string> targets{"'"};
	bool condition=true;
	while(condition){
		bool found=false;
		size_t location;
		int i;
		for(i=0; i<targets.size(); i++){
			location = input.find(targets[i]);
			if(location!=string::npos){
				found=true;
				check=true;
				break;
			}
		}
		if(found){
			input.erase(location, targets[i].size());
		}
		else{
			condition=false;
		}
	}
	return check;
}

void getStopWords(vector<string> &stopWords, string file){
	ifstream sourcefile(file);
	string temp;
	while(true){
		sourcefile>>temp;
		checkFormat(temp);
		stopWords.push_back(temp);
		if(sourcefile.eof()){
			break;
		}
	}
	std::cout<<"Found "<<stopWords.size()<<" stop words in "<<file<<"\n";
}

void saveStopWords(vector<string> &stopWords, string file){
	ofstream savefile(file);
	for(int i=0; i<stopWords.size(); i++){
		savefile<<stopWords[i]<<"\n";
	}
	std::cout<<"Saved "<<stopWords.size()<<" stop words to "<<file<<"\n";
}

int main(){
	vector<string> stopWords;
	string source = "stopwords.txt";
	getStopWords(stopWords, source);
	string save = "stopwordsFixed.txt";
	saveStopWords(stopWords, save);
}















