#include "State.h"
#include "Automata.h"
#include "NfaBuilder.h"
#include "tests/NfaToDfaTest.cpp"
#include "State.cpp"
#include "Automata.cpp"
#include "NfaBuilder.cpp"
#include "DFAConstructor.cpp"
#include "RulesParsing/FileParser.cpp"
#include "RulesParsing/FileParser.h"
#include "tests/test_scanner.cpp"


int main(int argv, char **argc) {
	// FileParser parser;
	// parser.parseFile("../example_rules.txt");
	// NFABuilder nfaBuilder;
	// Automata nfa = nfaBuilder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());
	nfa_to_dfa_test();

	std::string str = ";";
	char c = str[0];
	// Run the scanner test
	scanner_test();



	// print
	return 0;
}