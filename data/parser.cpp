#include <iostream>
#include <fstream>
#include <vector>
#include <string>


class wikiPage{
public:
	std::string timeStamp;
	bool featuredArticle;
	std::vector<std::string> category;
	std::string title;
	std::vector<std::string> body;
};


bool isTitle(char temp[100000]){
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


std::string fixTitle(char temp[100000]){
	int size = strlen(temp);
	int index=0;
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
	int newIndex=0;
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


bool isRedirect(char temp[100000]){
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


bool isFeatured(char temp[100000]){
	int size = strlen(temp);
	for(int i=0; i<size; i++){
		if(temp[i]=='{'){
			if(temp[i+1]=='{'){
				if(temp[i+2]=='f'){
					if(temp[i+3]=='e'){
						if(temp[i+4]=='a'){
							if(temp[i+5]=='t'){
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


bool isCategory(char temp[100000]){
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

std::string getCategory(char temp[100000]){
	int size = strlen(temp);
	bool condition = true;
	int index=0;
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
	int newIndex=0;
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

bool isUseless(char temp[100000]){
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

bool isTimeStamp(char temp[100000]){
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

std::string getTimeStamp(char temp[100000]){
	int size = strlen(temp);
	bool condition = true;
	int index=0;
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
	int newIndex=0;
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

bool titleSearch(std::string filename, std::string target, wikiPage &inputPage){
	bool condition = true;
	std::ifstream dataDump(filename);
	char temp[100000];
	bool found = false;
	while(condition){
		dataDump.getline(temp, 100000);
		if(isTitle(temp)){
			if(!isRedirect(temp)){
				std::string title = fixTitle(temp);
				if(title==target){
					found = true;
					inputPage.title = title;
					char tempTwo[100000];
					while(condition){
						dataDump.getline(tempTwo, 100000);
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
										inputPage.body.push_back(tempTwo);
									}
								}
							}
						}

					}

				}
			}
		}
	}
	if(found){
		return true;
	}
	else{
		return false;
	}
}


void read_file(std::string filename, std::vector<wikiPage> &input){
  char temp[100000];
  std::ifstream dataDump(filename);
  bool condition=true;
  std::string title;
  while(condition){
  	dataDump.getline(temp,100000);
  	if(isTitle(temp)){
  		title = fixTitle(temp);
  		condition=false;
  	}
  }
  int pageCount=0;
  int validPageCount=0;
  std::vector<std::string> titles;
  titles.push_back(title);
  while(validPageCount<1000000){
  	bool condition = true;
  	bool redirect = false;
  	bool featured = false;
  	bool categories = false;
  	bool gotTimeStamp = false;
  	std::vector<std::string> body;
  	std::vector<std::string> category;
  	std::string timeStamp;
  	while(condition){
  		dataDump.getline(temp, 100000);
  		if(isTitle(temp)){
  			std::string newTitle = fixTitle(temp);
  			titles.push_back(newTitle);
  			pageCount++;
  			condition=false;
  		}	
  		else{
  			if(isRedirect(temp)){
  				redirect=true;
  			}
  			else{
  				if(isFeatured(temp)){
  					featured=true;
  				}
  				else{
  					if(isCategory(temp)){
  						std::string cat = getCategory(temp);
  						category.push_back(cat);
  						categories=true;
  					}
  					else{
  						if(isUseless(temp)){
  						}
  						else{
  							if(isTimeStamp(temp)){
  								timeStamp = getTimeStamp(temp);
  								gotTimeStamp = true;
  							}
  							else{
  								body.push_back(temp);
  							}
  						}
  					}
  				}
  			}
  		}
  	}
  	wikiPage tempPage;
  	tempPage.title = titles[pageCount-1];
  	tempPage.body = body;
  	if(!redirect){
  		if(gotTimeStamp){
  			tempPage.timeStamp = timeStamp;
  		}
  		if(featured){
  			tempPage.featuredArticle=true;
  		}
  		if(categories){
  			tempPage.category=category;
  		}
  		input.push_back(tempPage);
  		validPageCount++;
  		std::cout<<validPageCount<<"\n";
  	}
  }
}


int main(){
	wikiPage page;
	std::cout<<"Enter the article you are looking for: ";
	std::string title;
	std::cin>>title;
	std::string filename = "enwiki-20160113-pages-articles.xml";
	std::cout<<"Searching...\n";
	if(titleSearch(filename, title, page)){
		std::cout<<"Found the article"<<std::endl;
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
	}
}
/*
int main(){
	std::string filename = "enwiki-20160113-pages-articles.xml";
	std::vector<wikiPage> pages;
	read_file(filename, pages);
	std::cout<<"-------------------------------------------\n";
	bool condition=true;
	while(condition){
		std::cout<<"-------------------------------------------\n";
		for(int i=0; i<pages.size(); i++){
			std::cout<<"-->["<<i<<"] "<<pages[i].title<<"\n";
		}
		std::cout<<"Enter your article choice: ";
		int input;
		std::cin>>input;
		for(int i=0; i<pages.size(); i++){
			if(i==input){
				if(pages[i].featuredArticle){
					std::cout<<"This article is featured!\n";
				}
			}
		}
		std::cout<<"-->[1] View body\n-->[2] View categories\n-->[3] View time stamp\n";
		std::cout<<"Enter you action choice: ";
		int inputTwo;
		std::cin>>inputTwo;
		switch(inputTwo){
			case 1:
			for(int i=0; i<pages[input].body.size(); i++){
				std::cout<<pages[input].body[i]<<"\n";
			}
			break;
			case 2:
			for(int i=0; i<pages[input].category.size(); i++){
				std::cout<<pages[input].category[i]<<"\n";
			}
			break;
			case 3:
			std::cout<<pages[input].timeStamp<<"\n";
		}
	}
}
*/











