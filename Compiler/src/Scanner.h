#pragma once
#include "SymbolTable.h"
#include <fstream>

class Scanner {
	SymbolTable* s1;
	string inputPath;
	bool parsedKeywords;
	bool parsedFunctions;
	bool parsedIdentifiers;
	bool parsedOperators;

	vector <pair<string, int> > functions;
	vector <pair<string, int> > keywords;

public:
	Scanner(SymbolTable* s) {
		s1 = s;
		parsedFunctions = parsedKeywords = parsedOperators = parsedIdentifiers = 0;
	}
	Scanner(string srcPath) {
		s1 = NULL;
		inputPath = srcPath;
		parsedFunctions = parsedKeywords = parsedOperators = parsedIdentifiers = 0;
	}
	Scanner(SymbolTable* s, string srcPath) {
		s1 = s;
		parsedFunctions = parsedKeywords = parsedOperators = parsedIdentifiers = 0;
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

	bool isAcceptableCharacter(char x) {
		if ((x > 64 && x < 91) || (x > 96 && x < 123) || (x > 47 && x < 58) || (x == 95) || (x==40) || (x==41))
			return 1;
		else
			return 0;
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

	bool findOperatorInToken(string token, string checkOperator) {
		int pos;
		
		pos = token.find(checkOperator);
		if (pos != string::npos) {
			if (pos != 0) {
				if (isAcceptableCharacter((char)token[pos - 1])) {
					if (token.c_str()[pos + 1] == '\0')
						return 1;
					if (isAcceptableCharacter((char)token[pos + checkOperator.length()]))
						return 1;
				}
			}
			else {
				if (token.length() == checkOperator.length())
					return 1;
				if (isAcceptableCharacter((char)token[pos + checkOperator.length()]))
					return 1;
			}
			return 0;

		}
	}

	pair<string, int> operatorChecker(string token) {
		//Arithmetic operator check (type = 0)
		bool flag = 0;
		string op = "+";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("+"), 0);
		op = "-";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("-"), 0);
		op = "*";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("*"), 0);
		op = "/";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("/"), 0);
		op = "%";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("%"), 0);
		
		//logical operators (type = 1)
		op = "&&";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("&&"), 1);
		op = "||";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("||"), 1);
		op = "==";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("=="), 1);
		op = "!=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("!="), 1);

		//assignment operators (type=2)
		op = "=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("="), 2);
		op = "+=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("+="), 2);
		op = "-=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("-="), 2);
		op = "*=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("*="), 2);
		op = "/=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("/="), 2);
		op = "%=";
		flag = findOperatorInToken(token, op);
		if (flag == 1)
			return make_pair(string("%="), 2);

		//No operator found
		return make_pair(string(""), -1);
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
		parsedKeywords = 1;
		outFile.close();
	}

	void filterFunctions() {
		ifstream inputFile(inputPath);
		string line;
		int lineNumber = 0;
		char * token;
		char * next_token = NULL;
		char delimiters[] = " \t\n";

		//vector <pair<string, int> > functions;

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
		parsedFunctions = 1;
		outFile.close();

	}

	string sanitize(string line){
		if (line[0] == '#')
			return string("");
		string ret = "";
		int start = -1;
		int end = -1;
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '\"' && start == -1)
			{
				start = i;
				continue;
			}
			if (line[i] == '\"' && end == -1)
			{
				end = i;
				break;
			}
		}
		ret = line;
		if(start!=-1)
			ret.replace(start, end - start + 1, " ");
		return ret;
	}

	void filterIdentifiers() {
		ifstream inputFile(inputPath);
		string line;
		int lineNumber = 0;
		char* token;
		char* next_token = NULL;
		//char* prev_token = NULL;
		char delimiters[] = " %><!+-*/=,();\t\n";
		string otherLeft = ",(";
		string otherRight = ",)";
		bool existsInFunctions = 0;
		bool existsInKeywords = 0;


		vector <pair<string, int> > identifiers;

		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				lineNumber++;
				line = sanitize(line);

				token = strtok_s((char*)line.c_str(), delimiters, &next_token);
		
				while (token) {
					existsInFunctions = 0;
					existsInKeywords = 0;
					//Parse the token
					char x = token[0];
					for (int k = 0; k < keywords.size(); k++) {
						if (string(token) == keywords[k].first) {
							existsInKeywords = 1;
							break;
						}
					}

					for (int k = 0; k < functions.size(); k++) {
						if (string(token) == functions[k].first) {
							existsInFunctions = 1;
							break;
						}
					}

					if(!existsInKeywords && !existsInFunctions)
						if ((x > 64 && x < 91) || (x > 96 && x < 123) || (x == 95))
							identifiers.push_back(make_pair(string(token), lineNumber));
					
					token = strtok_s(NULL, delimiters, &next_token);
						
				}
				
			}
			inputFile.close();
		}

		//Tokenized, output to file in tsv
		ofstream outFile("output_identifiers.txt", ofstream::out);
		for (int i = 0; i < identifiers.size(); i++) {
			outFile<< identifiers[i].first << "\t" << identifiers[i].second << endl;
		}
		parsedIdentifiers = 1;
		outFile.close();
	}
	
	void filterOperators() {
		ifstream inputFile(inputPath);
		string line;
		int lineNumber = 0;
		char* token;
		char* next_token = NULL;
		char delimiters[] = " \t\n";
		string otherLeft = ",(";
		string otherRight = ",)";

		vector <pair<string, int> > operators;



		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				lineNumber++;
				token = strtok_s((char*)line.c_str(), delimiters, &next_token);

				while (token) {
					//Parse the token
					pair<string, int> temp = operatorChecker(string(token));
					if (temp.second == 0 || temp.second == 1 || temp.second == 2)
						operators.push_back(make_pair(temp.first, lineNumber));
					
					token = strtok_s(NULL, delimiters, &next_token);
				}

			}
			inputFile.close();

		}

		//Tokenized, output to file in tsv
		ofstream outFile("output_operator.txt", ofstream::out);
		for (int i = 0; i < operators.size(); i++) {
			outFile << operators[i].first << "\t" << operators[i].second << endl;
		}
		parsedOperators = 1;
		outFile.close();
		
	}

	void parseIntoFiles() {
		filterKeywords();
		filterFunctions();
		filterIdentifiers();
		filterOperators();
	}

	void putIntoSymbolTable() {
		//Await Teacher's advice
		bool haveToPutKeywords = 0;

		ifstream inputFile("output_keyword.txt");
		string temp = "";
		if (inputFile.is_open()) {
			if (haveToPutKeywords)
			{
				inputFile >> temp;
				while (!inputFile.eof()) {
					if (s1->lookup(temp).second == -1)
						s1->insert_token(temp, string("KEYWORD"));
					inputFile >> temp;
					inputFile >> temp;
				}
			}
			inputFile.close();
		}
		
		inputFile.open("output_function.txt");
		temp = "";
		if (inputFile.is_open()) {
			inputFile >> temp;
			while (!inputFile.eof()) {
				if (s1->lookup(temp).second == -1)
					s1->insert_token(temp, string("FUNCTION"));
				inputFile >> temp;
				inputFile >> temp;
			}
			inputFile.close();
		}

		inputFile.open("output_identifiers.txt");
		temp = "";
		if (inputFile.is_open()) {
			inputFile >> temp;
			while (!inputFile.eof()) {
				if (s1->lookup(temp).second == -1)
					s1->insert_token(temp, string("IDENTIFIER"));
				inputFile >> temp;
				inputFile >> temp;
			}
			inputFile.close();
		}

		inputFile.open("output_operator.txt");
		temp = "";
		if (inputFile.is_open()) {
			inputFile >> temp;
			while (!inputFile.eof()) {
				if (s1->lookup(temp).second == -1)
					s1->insert_token(temp, string("OPERATOR"));
				inputFile >> temp;
				inputFile >> temp;
			}
			inputFile.close();
		}

		s1->print_tokens();

	}



};