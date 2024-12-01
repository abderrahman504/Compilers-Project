#include "State.h"
#include "Automata.h"
#include "NfaBuilder.h"
#include "RulesParsing/FileParser.h"
#include "tests/test_scanner.cpp"


int main(int argv, char **argc) {

	std::string str = ";";
	char c = str[0];
	// Run the scanner test
	scanner_test();



	// print
	return 0;
}