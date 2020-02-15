#pragma once
#include "SymbolTable.h"
#include <fstream>

class Scanner {
	SymbolTable* s1;
	string inputPath;
	string outputPath;
public:
	Scanner(SymbolTable* s) {
		s1 = s;
	}
	void fileRead(string srcPath) {
		inputPath = srcPath;

		ifstream inputFile(srcPath);

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
};