#include "ProgramAnalyzer.h"
#include <fstream>
#include <sstream>
#include <iostream>

ProgramAnalyzer::ProgramAnalyzer(Scanner scanner) : scanner(scanner) {}

void ProgramAnalyzer::analyzeFile(std::string file_path) {
	ifstream file(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open program file: example_program.txt\n");
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
	file.close();
}