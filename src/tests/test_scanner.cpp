#include "../NfaBuilder.h"
#include "../Automata.h"
#include "../Scanner.h"
#include "../RulesParsing/FileParser.h"
#include <fstream>
#include <sstream>
#include <iostream>


void scanner_test(){
	FileParser parser;
	parser.parseFile("example_rules.txt");
	NfaBuilder builder;
	Automata nfa = builder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());
	Scanner scanner = Scanner(nfa);

	// Open program file
	std::ifstream file("example_program.txt");
	if (!file.is_open()) {
		throw std::runtime_error("Could not open program file: example_program.txt");
	}
	std::string line;
	while(std::getline(file, line)){
		// seperate line by spaces
		std::stringstream ss(line);
		std::string word;
		while(std::getline(ss, word, ' ')){
			scanner.setInput(word);
			while(!scanner.isFinished()){
				string token = scanner.nextToken();
				string tk_type = scanner.getLastTokenType();
				if(tk_type == "")
					std::cout << "Invalid\n";
				else
					std::cout << tk_type << "\n";
			}
		}
	}
}