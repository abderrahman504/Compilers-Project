#include "State.h"
#include "Automata.h"
#include "NfaBuilder.h"
#include "RulesParsing/FileParser.h"

int main(int argv, char **argc) {
	FileParser parser;
	parser.parseFile("example_rules.txt");
	NFABuilder nfaBuilder;
	Automata nfa = nfaBuilder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());
	
	// print
	return 0;
}