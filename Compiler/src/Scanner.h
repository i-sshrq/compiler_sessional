#pragma once
#include "SymbolTable.h"
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
	//KEYWORD CHECKER Returns the matched keyword or empty string
	string keywordChecker(string token, string otherLeft, string otherRight) {
		vector<string> actualKeywords= {"if", "else", "int", "for", "while", "break", "continue", "float", "double", "char", "return"};
		bool leftFlag = 0;
		bool rightFlag = 0;

		for (int i = 0; i < actualKeywords.size(); i++) {
			int pos = token.find(actualKeywords[i]);
			if (pos != string::npos) {
				//check absolute match
				if (token.size() == actualKeywords[i].size()) return actualKeywords[i];
				//check left
				//(int
				if (pos != 0 && token.c_str()[pos + actualKeywords[i].length()] == '\0') {
					for (int j = 0; j < otherLeft.length(); j++) {
						if (token[pos - 1] == otherLeft[j])
							return actualKeywords[i];
					}
				}
				//(int)
				if (pos != 0 && token.c_str()[pos + actualKeywords[i].length()] != '\0') {
					for (int j = 0; j < otherLeft.length(); j++) {
						if (token[pos - 1] == otherLeft[j])
							leftFlag = 1;
					}
					if (leftFlag == 1)
						for (int j = 0; j < otherRight.length(); j++) {
							if (token[pos + actualKeywords[i].length()] == otherRight[j])
								rightFlag = 1;
						}
				}
				if (leftFlag == 1 && rightFlag == 1) return actualKeywords[i];

				//check right
				//int)
				if (pos == 0 && token.c_str()[pos + actualKeywords[i].length()] != '\0') {
					for (int j = 0; j < otherRight.length(); j++) {
						if (token[pos + actualKeywords[i].length()] == otherRight[j])
							return actualKeywords[i];
					}
				}
				//(int) already checked
			}
		}
		return string("");
	}

	//FUNCTION CHECKER, returns function name only
	string functionChecker(string token) {
		//specifically not counted as functions if matched with these
		vector <string> actualKeywords = { "if", "for", "while", "else if" };
		int pos = token.find("(");
		if (pos != 0 && pos!=string::npos) {
			string word = token.substr(0, pos);
			for (int i = 0; i < actualKeywords.size(); i++) {
				if (word == actualKeywords[i])
					return string("");
			}
			return word;
		}
		return string("");
	}

	//PRIMITIVE CHECKER, returns primitive type name
	string primitiveChecker(string token, string otherLeft, string otherRight) {
		vector <string> actualPrimitives = { "long long int", "long int", "long double", "int", "char", "float", "double" };

		bool leftFlag = 0;
		bool rightFlag = 0;

		for (int i = 0; i < actualPrimitives.size(); i++) {
			int pos = token.find(actualPrimitives[i]);
			if (pos != string::npos) {
				//check absolute match
				if (token.size() == actualPrimitives[i].size()) return actualPrimitives[i];
				//check left
				//(int
				if (pos != 0 && token.c_str()[pos+actualPrimitives[i].length()]=='\0') {
					for (int j = 0; j < otherLeft.length(); j++) {
						if (token[pos - 1] == otherLeft[j])
							return actualPrimitives[i];
					}
				}
				//(int)
				if (pos != 0 && token.c_str()[pos + actualPrimitives[i].length()] != '\0') {
					for (int j = 0; j < otherLeft.length(); j++) {
						if (token[pos - 1] == otherLeft[j])
							leftFlag = 1;
					}
					if(leftFlag==1)
						for (int j = 0; j < otherRight.length(); j++) {
							if (token[pos + actualPrimitives[i].length()] == otherRight[j])
								rightFlag = 1;
						}
				}
				if (leftFlag == 1 && rightFlag == 1) return actualPrimitives[i];
				
				//check right
				//int)
				if (pos == 0 && token.c_str()[pos + actualPrimitives[i].length()] != '\0') {
					for (int j = 0; j < otherRight.length(); j++) {
						if (token[pos + actualPrimitives[i].length()] == otherRight[j])
							return actualPrimitives[i];
					}
				}
				//(int) already checked
			}
		}
		return string("");
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
					string otherLeft = "(,";
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

		vector <pair<string, int> > functions;

		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				lineNumber++;
				int pos = line.find("(");
				if (pos != string::npos) {
					
					token = strtok_s((char*)line.c_str(), delimiters, &next_token);
					while (token) {
						//Parse the token
						string temp = functionChecker(string(token));
						if (temp.length() != 0) {
							functions.push_back(make_pair(temp, lineNumber));
						}
						token = strtok_s(NULL, delimiters, &next_token);
					}
				}
			}
			inputFile.close();
		}

		//Tokenized, output to file in tsv
		ofstream outFile("output_function.txt", ofstream::out);
		for (int i = 0; i < functions.size(); i++) {
			outFile << functions[i].first << "\t" << functions[i].second << endl;
		}
		outFile.close();

	}

	void filterPrimitives() {
		ifstream inputFile(inputPath);
		string line;
		int lineNumber = 0;
		char* token;
		char* next_token = NULL;
		char delimiters[] = " \t\n";
		string otherLeft = ",(";
		string otherRight = ",)";

		vector <pair<string, int> > primitives;

		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				lineNumber++;
				token = strtok_s((char*)line.c_str(), delimiters, &next_token);
		
				while (token) {
					//Parse the token
					string temp = primitiveChecker(string(token), otherLeft, otherRight);
					if (temp.length() != 0) {
						primitives.push_back(make_pair(temp, lineNumber));
					}
					token = strtok_s(NULL, delimiters, &next_token);
				}
				
			}
			inputFile.close();
		}

		//Tokenized, output to file in tsv
		ofstream outFile("output_primitives.txt", ofstream::out);
		for (int i = 0; i < primitives.size(); i++) {
			outFile << primitives[i].first << "\t" << primitives[i].second << endl;
		}
		outFile.close();
	}
	

};