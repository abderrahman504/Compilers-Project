#include <string>
#include "RulesParsing/FileParser.h"
#include <stdio.h>

void fileParserTest(){
	std::string test_file = "example_rules.txt";
	FileParser parser;
	parser.parseFile(test_file);
	for(auto entry : parser.getRegularDefinitions()){
		printf("%s : \n%s\n", entry.first.c_str(), entry.second.c_str());
	}
	for(auto entry : parser.getRegularExpressions()){
		printf("%s : \n%s\n", entry.first.c_str(), entry.second.c_str());
	}
	printf("Keywords: \n");
	for(auto entry : parser.getKeywords()){
		printf("%s\n", entry.c_str());
	}
	printf("Punctuations: \n");
	for(auto entry : parser.getPunctuations()){
		printf("%s\n", entry.c_str());
	}


	return;
}