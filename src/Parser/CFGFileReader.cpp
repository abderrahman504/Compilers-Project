#include "CFGFileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>



CFGFileReader::RulesMap CFGFileReader::readCFG(const std::string& filename, std::string& out_start_symbol) {
    RulesMap rules;
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line, lhs, current_rhs;
    bool is_rhs_continuing = false;

    while (std::getline(infile, line)) {
        // Trim whitespace
        line = trim(line);

        if (line.empty() || line[0] == '#') continue; // Skip empty or comment lines

        if (!is_rhs_continuing) {
            auto pos = line.find("::=");
            if (pos == std::string::npos) {
                throw std::runtime_error("Invalid production rule format: " + line);
            }

            lhs = trim(line.substr(0, pos));
            current_rhs = line.substr(pos + 3);
        } else {
            current_rhs += " " + line;
        }

        if (line.back() == '\\') {
            is_rhs_continuing = true;
            current_rhs.pop_back(); // Remove the backslash
        } else {
            is_rhs_continuing = false;
            parseAndStoreRule(lhs, current_rhs, rules);
        }
    }

    infile.close();
    return rules;
}

std::string CFGFileReader::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void CFGFileReader::parseAndStoreRule(const std::string& lhs, const std::string& rhs, RulesMap& rules) {
    std::vector<std::string> options;
    std::stringstream ss(rhs);
    std::string part;

    while (std::getline(ss, part, '|')) {
        options.push_back(trim(part));
    }

    if (rules.find(lhs) == rules.end()) {
        rules[lhs] = options;
    } else {
        rules[lhs].insert(rules[lhs].end(), options.begin(), options.end());
    }
}


