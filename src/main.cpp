#include "lexical_analysis/lexical_analysis.h"
#include "Parser/syntax_parsing.h"
#include <iostream>

int main(int argv, char **argc)
{
	// PHASE 1

	ProgramAnalyzer lexical_analyzer("example_rules.txt");
	vector<TableEntry> tokens = lexical_analyzer.analyzeFile("example_program.txt");


	// PHASE 2
	Parser syntax_parser("example_grammer_LL1.txt");

	return 0;
}