#pragma once
#include <string>
#include <list>

using namespace std;

//Hash function for token insertion
int hashFunc(string name) {
	int length = name.length();
	if (length == 1)
		return (name[0] * 23) % 101;
	else if (length % 2 == 0)
		return ((name[length - 2] + name[length - 1])*23)%101;
	else
		return ((name[0] + name[1]) * 23) % 101;
}


class SymbolInfo {
private:
	string symbol;
	string type;
public:
	SymbolInfo(string sym, string typ) {
		symbol = sym;
		type = typ;
	}
	string getSymbol() {
		return symbol;
	}
	string getType() {
		return type;
	}
};


class SymbolTable {
private:
	list<SymbolInfo>* table;
public:

	//Constructors and destructors for the table
	SymbolTable() {
		table = new list<SymbolInfo>[101];
	}
	~SymbolTable() {
		for (int i = 0; i < 101; i++)
			table[i].clear();
	}

	//Insert each token into the proper place in the table
	void insert_token(string sym, string typ) {
		/// TODO confusion
		int key = hashFunc(typ);
		table[key].push_back(SymbolInfo(sym, typ));
	}

	//Prints the entire table
	void print_tokens() {
		for (int i = 0; i < 101; i++) {
			cout << i << " ------> ";
			for (list<SymbolInfo>::iterator j = table[i].begin(); j != table[i].end(); j++) {
				string tempSym = (*j).getSymbol();
				string tempType = (*j).getType();
				cout << "<" << tempSym << ", " << tempType << ">    ";
			}
			cout << endl;
		}
	}


	
};