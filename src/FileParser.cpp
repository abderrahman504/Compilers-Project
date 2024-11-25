#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>

// Parses the input file
void FileParser::parseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::string line;
    std::regex defRegex("^([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*(.+)$"); // Regular Definition: LHS = RHS
    std::regex exprRegex("^([a-zA-Z_][a-zA-Z0-9_]*)\\s*:\\s*(.+)$"); // Regular Expression: LHS: RHS
    std::regex keywordRegex("^\\{(.+)\\}$");                        // Keywords: { }
    std::regex punctuationRegex("^\\[(.+)\\]$");                    // Punctuation: [ ]

    while (std::getline(file, line)) {
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), ""); // Trim whitespace

        std::smatch match;
        if (std::regex_match(line, match, defRegex)) {
            // Regular Definition
            regularDefinitions[match[1]] = match[2];
        } else if (std::regex_match(line, match, exprRegex)) {
            // Regular Expression
            regularExpressions[match[1]] = match[2];
        } else if (std::regex_match(line, match, keywordRegex)) {
            // Keywords
            std::stringstream ss(match[1]);
            std::string keyword;
            while (std::getline(ss, keyword, ' ')) {
                keywords.push_back(keyword);
            }
        } else if (std::regex_match(line, match, punctuationRegex)) {
            // Punctuation
            std::stringstream ss(match[1]);
            std::string punctuation;
            while (std::getline(ss, punctuation, ' ')) {
                punctuations.push_back(punctuation);
            }
        } else {
            // Operators or invalid format
            operators.push_back(line);
        }
    }

    file.close();
}

// Getters
const std::unordered_map<std::string, std::string>& FileParser::getRegularDefinitions() const {
    return regularDefinitions;
}

const std::unordered_map<std::string, std::string>& FileParser::getRegularExpressions() const {
    return regularExpressions;
}

const std::vector<std::string>& FileParser::getKeywords() const {
    return keywords;
}

const std::vector<std::string>& FileParser::getPunctuations() const {
    return punctuations;
}

const std::vector<std::string>& FileParser::getOperators() const {
    return operators;
}
