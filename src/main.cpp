#include "lexical_analysis/lexical_analysis.h"
#include "Parser/syntax_parsing.h"
#include <iostream>

int main(int argv, char **argc)
{
	// PHASE 1

	ProgramAnalyzer lexical_analyzer("example_rules.txt");
	vector<TableEntry> tokens = lexical_analyzer.analyzeFile("example_program.txt");


	// PHASE 2

	Grammar grammer("example_grammer_LL1.txt");
	grammer.computeFirsts();
	grammer.computeFollows();
	if(!grammer.isLL1()){
		cout << "Grammer is not LL(1). Aborting.\n";
		return 0;
	}

	return 0;
}