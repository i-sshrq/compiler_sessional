#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"

using namespace std;

int main() {
	SymbolTable * st1 = new SymbolTable();
	Scanner * s = new Scanner(st1);

	s->fileRead("Resources/input.txt");

	return 0;
}