#include "GrammerFileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}



void GrammerFileReader::readRules(string file_path)
{
	std::ifstream infile(file_path);
    if (!infile) {
        throw std::runtime_error("Unable to open file: " + file_path + "\n");
    }

    std::string line, lhs, current_rhs;
    bool is_rhs_continuing = false;
	bool first_lhs = true;

    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Skip empty or comment lines

        if (!is_rhs_continuing) {
            auto pos = line.find("::=");
            if (pos == std::string::npos) {
                throw std::runtime_error("Invalid production rule format: " + line);
            }

            lhs = trim(line.substr(0, pos));
            current_rhs = line.substr(pos + 3);

            // Add LHS to nonTerminals
            non_terminals.insert(lhs);
			if(first_lhs){
				first_lhs = false;
				start_symbol = lhs;
			}
            // Erase this symbol from terminals set in case it was mistakenly added
            terminals.erase(lhs);
        } else {
            current_rhs += " " + line;
        }

        if (line.back() == '\\') {
            is_rhs_continuing = true;
            current_rhs.pop_back(); // Remove the backslash
        } else {
            is_rhs_continuing = false;

            // Parse the RHS into individual productions
            std::vector<std::vector<std::string>> rhs_productions;
            std::stringstream ss(current_rhs);
            std::string option;
            while (std::getline(ss, option, '|')) {
                std::vector<std::string> tokens;
                std::stringstream option_stream(trim(option));
                std::string token;

                while (option_stream >> token) {
                    // Check if the token is a terminal
					if(non_terminals.count(token) == 0){
						if(token != epsilon) terminals.insert(token);
					}
                    tokens.push_back(token);
                }
                rhs_productions.push_back(tokens);
            }

            // Add to productions map
            productions[lhs].insert(productions[lhs].end(), rhs_productions.begin(), rhs_productions.end());
        }
    }

    infile.close();
}

string GrammerFileReader::getStartSymbol(){
	return start_symbol;
}
unordered_set<string> GrammerFileReader::getTerminals(){
	return terminals;
}
unordered_set<string> GrammerFileReader::getNonTerminals(){
	return non_terminals;
}
unordered_map<string, vector<vector<string>>> GrammerFileReader::getProductions(){
	return productions;
}


