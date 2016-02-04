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


class word{
public:
	double freq;
	double logFreq;
	int count;
	string name;
	word(string input){
		name=input;
		count=1;
	}
	word(string input, int inputTwo){
		name=input;
		count=inputTwo;
	}
	word(){}
	friend ostream& operator<<(ostream& os, word& w);
};

double process(vector<word> &input){
	double total=0;
	for(int i=0; i<input.size(); i++){
		total+=input[i].count;
	}
	for(int i=0; i<input.size(); i++){
		input[i].freq = double(input[i].count/total);
		input[i].logFreq = log(input[i].freq);
	}
	std::cout<<"Total words read: "<<total<<"\n";
	return total;
}

ostream& operator<<(ostream& os, word& w)
{
    os <<"Word:\t\t"<<w.name<<"\nCount:\t\t"<<w.count<<"\nFrequency:\t"<<w.freq<<"\nLog Frequency:\t"<<w.logFreq<<"\n";
    return os;
}

bool checkFormat(string &input){
	bool check = false;
	string target = "\"";
	vector<string> targets{"\"", "'", "[","]",".",",",":","!",")","(","*",";","?","#","_"};
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

bool haveIt(vector<word> &words, string &input){
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);

	for(int i=0; i<words.size(); i++){
		if(words[i].name==input){
			words[i].count++;
			return true;
		}
	}
	bool checkAgain = checkFormat(input);
	if(checkAgain){
		for(int i=0; i<words.size(); i++){
			if(words[i].name==input){
				words[i].count++;
				return true;
			}
		}
	}
	words.push_back(input);
	return true;
}

bool wordCompare(word A, word B){
	int aVal = A.count;
	int bVal = B.count;
	return (aVal>bVal);
}

void sortWords(vector<word> &words){
	std::sort(words.begin(), words.end(), wordCompare);
}


//Take in multiple txt files and create a single bag of words
void getDataSingle(vector<word> &words, vector<string> files){
	for(int i=0; i<files.size(); i++){
		std::cout<<"Reading "<<files[i]<<"\n";
		ifstream text(files[i]);
		std::string temp;
		while(true){
			text>>temp;
			haveIt(words, temp);
			if(text.eof()){
				break;
			}
		}
	}
	sortWords(words);
	process(words);
	std::cout<<"Total distinct word count: "<<words.size()<<"\n";
}

//Take in multiple txt files and create a bag of words for each
void getDataMultiple(vector<vector<word>> &words, vector<string> files){
	for(int i=0; i<files.size(); i++){
		std::cout<<"Reading "<<files[i]<<"\n";
		vector<word> wordVec;
		ifstream text(files[i]);
		std::string temp;
		while(true){
			text>>temp;
			haveIt(wordVec, temp);
			if(text.eof()){
				break;
			}
		}
		sortWords(wordVec);
		process(wordVec);
		words.push_back(wordVec);
	}
}

//Save to a text file, each row is its own word
void saveDataSingle(vector<word> &words, string file){
	ofstream output(file);
	for(int i=0; i<words.size(); i++){
		output<<words[i].name<<" "<<words[i].count<<" "<<words[i].freq<<" "<<words[i].logFreq<<" ";
	}
	cout<<"Saved word data to "<<file<<"\n";
}

void saveDataMultiple(vector<vector<word>> &words, string file, vector<string> fileNames){
	ofstream output(file);
	for(int i=0; i<words.size(); i++){
		output<<fileNames[i]<<"--> ";
		for(int j=0; j<words[i].size(); j++){
			output<<words[i][j].name<<" "<<words[i][j].count<<" "<<words[i][j].freq<<" "<<words[i][j].logFreq<<" ";
		}
		output<<"\n\n";
	}
}

int main(){
	vector<vector<word>> words;
	//Took out monte cristo and war&peace because they took too long to compile
	vector<string> files{"alice.txt","holmes.txt","finn.txt","slave.txt","pride.txt","treasure.txt","peter.txt","dracula.txt"};
	getDataMultiple(words, files);
	string file = "testMultiple.txt";
	saveDataMultiple(words, file, files);
}

/*
int main(){
	vector<word> words;
	vector<string> files{"alice.txt","holmes.txt","finn.txt","slave.txt","pride.txt","war.txt","monte.txt", "treasure.txt","peter.txt","dracula.txt"};
	getData(words, files);
	string file = "words.txt";
	saveDataColumn(words, file);
}
*/









