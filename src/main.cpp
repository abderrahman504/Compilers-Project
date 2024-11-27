#include "State.h"
#include "Automata.h"
#include "RulesParsing/FileParser.h"


int main(int argv, char **argc) {
	FileParser parser = FileParser();
	parser.parseFile("example_rules.txt");
	return 0;
}