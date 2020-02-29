#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"

using namespace std;

int main() {
	Scanner * s = new Scanner("input.txt");
	s->filterKeywords();
	s->filterFunctions();
	return 0;
}