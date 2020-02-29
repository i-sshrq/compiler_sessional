#pragma once
#include "SymbolTable.h"
#include <stack>
#include <fstream>

class Scanner {
	SymbolTable* s1;
	string inputPath;
public:
	Scanner(SymbolTable* s) {
		s1 = s;
	}
	Scanner(string srcPath) {
		s1 = NULL;
		inputPath = srcPath;
	}
	Scanner(SymbolTable* s, string srcPath) {
		s1 = s;
		inputPath = srcPath;
	}
	//Returns the matched keyword or empty string
	string keywordChecker(string token, string otherLeft, string otherRight) {
		vector<string> actualKeywords= {"if", "else", "int", "for", "while", "break", "continue", "float", "double", "char", "return"};
		for (int i = 0; i < actualKeywords.size(); i++) {
			size_t pos = token.find(actualKeywords[i]);
			if (pos != string::npos) {
				//on perfect match
				if (token == actualKeywords[i])
					return token;
				//found, but look for nearby characters RIGHT
				if (pos == 0) {
					for (int j = 0; j < otherRight.length(); j++) {
						if (token[actualKeywords[i].size()] == otherRight[j])
							return actualKeywords[i];
					}
				}
				//found, but look for nearby characters LEFT
				else {
					//cout << "POS = " << pos << endl;
					for (int j = 0; j < otherLeft.length(); j++) {
						if (token[pos - 1] == otherLeft[j])
							return actualKeywords[i];
					}
				}
			}
		}
		return string("");
	}

	string functionChecker(string token, string otherLeft, string otherRight) {
		//Check functions by parenthesis matching
		stack <char> stack_p;
		string actualKeywords[] = { "if", "for", "while", "else if" };



	}





	void assignment1() {
	

		ifstream inputFile(inputPath);

		vector <string> wordBuffer;
		string line = "", word = "";

		int i;
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				cout << "Input : " << line << endl;
				cout << "Output : " << endl;
				i = 0;
				//Splits each line into words, and puts them into wordBuffer
				while (i <= line.length()) {
					if (line[i] != ' ' && i != line.length()) {
						word = word + line[i];
						i++;
					}
					else {
						wordBuffer.push_back(word);
						word.clear();
						i++;
					}
				}

				switch (line[0]) {
				case 'I':
					s1->insert_token(wordBuffer[1], wordBuffer[2]);
					break;
				case 'L':
					s1->lookup(wordBuffer[1]);
					break;
				case 'P':
					s1->print_tokens();
					break;
				case 'D':
					s1->delete_token(wordBuffer[1]);
					break;
				default:
					cout << "Invalid syntax" << endl;
					break;
				}




				//Clears wordBuffer before parsing next line
				wordBuffer.clear();
				cout << endl << endl;
			}
		}
		else {
			cout << "input.txt File was not found" << endl;
		}

		//Closing all file streams for safety
		if (inputFile.is_open()) {
			inputFile.close();
		}

	}

	void filterKeywords() {
		//vector <string> wordBuffer;
		ifstream inputFile(inputPath);
		string line;
		char delimiters[] = " \t\n";
		char* token;
		char* next_token = NULL;
		int lineNumber = 0;
		vector< pair<string, int> > keywords;
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				lineNumber++;
				token = strtok_s((char*)line.c_str(), delimiters, &next_token);
				while (token){
					//Parse the token
					string otherRight = "(),;";
					string otherLeft = "(),";
					string temp = keywordChecker(token, otherLeft, otherRight);
					if (temp.length() != 0) {
						//cout << "FOUND " << temp << endl;
						keywords.push_back(make_pair(temp, lineNumber));
					}					
					token = strtok_s(NULL, delimiters, &next_token);
				}
			}
			inputFile.close();
		}

		

		//Tokenized, output to file in tsv
		ofstream outFile("output_keyword.txt", ofstream::out);
		for (int i = 0; i < keywords.size(); i++) {
			outFile << keywords[i].first << "\t" << keywords[i].second << endl;
		}
		outFile.close();
	}

	void filterFunctions() {
		ifstream inputFile(inputPath);
		string line;
		int lineNumber = 0;
		char * token;
		char * next_token = NULL;
		char delimiters[] = " \t\n";

		vector < pair<string, int> > functions;

		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				int pos = line.find("(");
				if (pos != string::npos) {
					lineNumber++;
					token = strtok_s((char*)line.c_str(), delimiters, &next_token);
					while (token) {
						//Parse the token
						string otherRight = "},";
						string otherLeft = "{,";
						string temp = functionChecker(token, otherLeft, otherRight);
						if (temp.length() != 0) {
							functions.push_back(make_pair(temp, lineNumber));
						}
						token = strtok_s(NULL, delimiters, &next_token);
					}
				}
			}

			inputFile.close();
		}
	}
	

};