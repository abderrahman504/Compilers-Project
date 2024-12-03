#include "State.h"
#include "Automata.h"
#include "NfaBuilder.h"
#include "tests/NfaToDfaTest.cpp"
#include "RulesParsing/FileParser.h"
#include "ProgramAnalyzer.h"
#include "DFAConstructor.h"
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
	dfa = dfa_constructor.minimizeDFA(dfa);

	// Analyze a program with the automata.
	//std::cout << "Analyses using NFA...\n";
	//Scanner scanner = Scanner(nfa);
	//ProgramAnalyzer analyzer = ProgramAnalyzer(scanner);
	//analyzer.analyzeFile("example_program.txt");

	std::cout << "Analysis using DFA...\n";
	Scanner scanner2 = Scanner(dfa);
	ProgramAnalyzer analyzer2 = ProgramAnalyzer(scanner2);
	analyzer2.analyzeFile("example_program.txt");

	// Print Transition Table
	dfa_constructor.printTransitionTable(dfa);

	return 0;
}