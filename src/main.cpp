#include "lexical_analysis/lexical_analysis.h"
#include <iostream>

int main(int argv, char **argc)
{
	// Parse rules file
	FileParser parser;
	parser.parseFile("example_rules.txt");

	// Construct a NFA
	NfaBuilder builder;
	Automata nfa = builder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());

	// Construct a minimal DFA
	DFAConstructor dfa_constructor;
	Automata dfa = dfa_constructor.constructDFA(nfa);
	nfa.freeStates();
	Automata min_dfa = dfa_constructor.minimizeDFA(dfa);
	dfa.freeStates();

	std::cout << "Analysis using DFA...\n";
	Scanner scanner = Scanner(min_dfa);
	ProgramAnalyzer analyzer = ProgramAnalyzer(scanner);
	vector<TableEntry> tokens = analyzer.analyzeFile("example_program.txt");

	return 0;
}