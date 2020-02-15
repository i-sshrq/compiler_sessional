#pragma once
#include <string>
#include <list>

using namespace std;

//Hash function for token insertion
int hashFunc(string symbol) {
	int length = symbol.length();
	if (length == 1)
		return (symbol[0] * 23) % 101;
	else if (length % 2 == 0)
		return ((symbol[length - 2] + symbol[length - 1])*23)%101;
	else
		return ((symbol[0] + symbol[1]) * 23) % 101;
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

	bool operator == (SymbolInfo const& s1) {
		if (symbol == s1.symbol && type == s1.type)
			return 1;
		else
			return 0;
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

	//INSERT each token into the proper place in the table
	void insert_token(string sym, string typ) {
		/// TODO confusion
		int key = hashFunc(sym);
		table[key].push_back(SymbolInfo(sym, typ));
		cout << "<" << sym << ", " << typ << "> inserted at position (" << key << ", " << table[key].size() - 1 << ")" << endl;
	}

	//PRINTs the entire table
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

	//LOOKUP, returns pair for later use
	pair<int, int> lookup(string sym) {
		int y;
		for (int x = 0; x < 101; x++) {
			y = -1;
			list<SymbolInfo>::iterator it;
			for (it = table[x].begin(); it != table[x].end(); ++it) {
				y++;
				if (it->getSymbol() == sym) {
					cout << "Found at (" << x << ", " << y << ")" << endl;
					return make_pair(x, y);
				}
			}
		}
	}

	//DELETE
	void delete_token(string sym) {
		int y;
		
		for (int x = 0; x < 101; x++) {
			list<SymbolInfo>::iterator it;
			y = -1;
			for (it = table[x].begin(); it != table[x].end(); it++) {
				y++;
				if (it->getSymbol() == sym) {
					table[x].erase(it);
					cout << "Deleted from (" << x << ", " << y << ")" << endl << endl;
					return;
				}
			}
		}
	}

};