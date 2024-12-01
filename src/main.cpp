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

int main(int argv, char **argc) {
	// FileParser parser;
	// parser.parseFile("../example_rules.txt");
	// NFABuilder nfaBuilder;
	// Automata nfa = nfaBuilder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());
	nfa_to_dfa_test();
	// print
	return 0;
}