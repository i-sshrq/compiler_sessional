#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"

using namespace std;

int main() {
	SymbolTable * s1 = new SymbolTable();
	Scanner* s = new Scanner(s1, "input.txt");
	s->parseIntoFiles();
	s->putIntoSymbolTable();
	return 0;
}