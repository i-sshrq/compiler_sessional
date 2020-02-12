#include <iostream>
#include "SymbolTable.h"

using namespace std;

int main() {
	SymbolTable * s1 = new SymbolTable();
	s1->insert_token("123", "NUMBER");
	s1->insert_token("231", "NUMBER");
	s1->insert_token("foo", "FUNCTION");
	s1->insert_token("foo", "FUNCTION");
	//s1->print_tokens();
	s1->lookup("foo");
	s1->delete_token("foo");

}