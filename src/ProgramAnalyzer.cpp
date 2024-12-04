#include "ProgramAnalyzer.h"
#include <fstream>
#include <sstream>
#include <iostream>

TableEntry::TableEntry(string symbol, string type){
	this->symbol = symbol;
	this->type = type;
}



ProgramAnalyzer::ProgramAnalyzer(Scanner scanner) : scanner(scanner) {}

void ProgramAnalyzer::analyzeFile(std::string file_path) {
	
	vector<TableEntry> symbol_table;

	ifstream file(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open program file: example_program.txt\n");
	}
	int line_count = 0;
	std::string line;
	while(std::getline(file, line)){
		line_count++;
		int char_count = 0;
		// seperate line by spaces
		std::stringstream ss(line);
		std::string word;
		while(std::getline(ss, word, ' ')){
			char_count++; //add 1 for the ' ' character
			int word_start = 0;
			scanner.setInput(word);
			while(!scanner.isFinished()){
				string token = scanner.nextToken();
				string tk_type = scanner.getLastTokenType();
				if(tk_type == ""){
					std::cout << "Invalid token at line:" << line_count << " col:" << char_count << std::endl;
					// If this invalid token is the last in the input then skip a character and try again.
					if(scanner.isFinished())
						scanner.setInput(word.substr(++word_start));
				}
				else{
					std::cout << tk_type << "\n";
					TableEntry entry(token, tk_type);
					symbol_table.push_back(entry);
				}
				char_count += token.length();
			}
		}
	}
	file.close();
}