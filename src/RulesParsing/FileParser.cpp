#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>

using namespace std;

// Parses the input file
void FileParser::parseFile(const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open rules file: " + filePath);
    }

    string line;
    // For def and expr lines the '=' and ':' must not be preceeded by a '\' character.
    std::regex defRegex("^(.+)\\s*[^\\\\]=\\s*(.+)$"); // Regular Definition: LHS = RHS
    std::regex exprRegex("^([a-zA-Z_][a-zA-Z0-9_]*)\\s*[^\\\\]:\\s*(.+)$"); // Regular Expression: LHS: RHS
    std::regex keywordRegex("^\\{(.+)\\}$");                        // Keywords: { }
    std::regex punctuationRegex("^\\[(.+)\\]$");                    // Punctuation: [ ]

    while (std::getline(file, line)) {
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), ""); // Trim whitespace

        std::smatch match;
        if (std::regex_match(line, match, defRegex)) {
            // Regular Definition
            string rhs = std::regex_replace(match[2].str(), std::regex("\\s"), "");
            vector<string> definitions = processDefinitionRhs(rhs);
            regularDefinitions[match[1]] = definitions;
        } else if (std::regex_match(line, match, exprRegex)) {
            // Regular Expression
            // EXPAND ANY DIFINITIONS BEFORE STORING THE EXPRESSION
            string expanded_rhs = processExpressionRhs(match[2]);
            regularExpressions[match[1]] = expanded_rhs;
        } else if (std::regex_match(line, match, keywordRegex)) {
            // Keywords
            stringstream ss(match[1]);
            string keyword;
            while (std::getline(ss, keyword, ' ')) {
                keywords.push_back(keyword);
            }
        } else if (std::regex_match(line, match, punctuationRegex)) {
            // Punctuation
            // PUNTUATIONS SHOULD AUTOMATICALLY INCLUDE ' ' AND '\n' SO AN NFA CAN SKIP THEM
            stringstream ss(match[1]);
            string punctuation;
            while (std::getline(ss, punctuation, ' ')) {
                punctuations.push_back(punctuation);
            }
        }
    }

    file.close();
}

// Getters
const std::unordered_map<string, vector<string>>& FileParser::getRegularDefinitions() const {
    return regularDefinitions;
}

const std::unordered_map<string, string>& FileParser::getRegularExpressions() const {
    return regularExpressions;
}

const vector<string>& FileParser::getKeywords() const {
    return keywords;
}

const vector<string>& FileParser::getPunctuations() const {
    return punctuations;
}


vector<string> FileParser::processDefinitionRhs(string rhs)
{
    vector<string> definitions = vector<string>();
    int idx = -1;
    while (++idx < rhs.size())
    {
        char c = rhs[idx];
        // If c is followed by a dash and another character then this is a range of characters
        if(idx < rhs.size()-2 && rhs[idx+1] == '-'){
            for(char i = c; i <= rhs[idx+2]; i++){
                string str("a");
                str[0] = i;
                definitions.push_back(str);
            }
            idx += 2;
            continue;
        }
        else{ //Otherwise take all characters until | or end
            if (c == '|') continue;

            string str("");
            str += c;
            idx++;
            while(idx < rhs.size() && rhs[idx] != '|'){
                c = rhs[idx];
                str += c;
                idx++;
            }
            definitions.push_back(str);
            // decrement idx if | was encountered
            idx--;
        }
    }

    return definitions;
}


string FileParser::processExpressionRhs(string rhs){
    unordered_map<string, string> substitutions = unordered_map<string, string>();
    for (auto it : regularDefinitions){
        // Check if it.first exists in rhs and replace it with the strings in it.second piped up
        string def_name = it.first;
        vector<string> def_values = it.second;
        string def_values_piped = "(";
        for (auto val : def_values){
            def_values_piped += val + "|";
        }
        def_values_piped.pop_back(); // Remove the last pipe
        def_values_piped += ")";
        substitutions[def_name] = def_values_piped;
    }

    // Replace all definitions in the rhs
    for (auto it : substitutions){
        string def_name = it.first;
        string def_values_piped = it.second;
        rhs = regex_replace(rhs, regex(def_name), def_values_piped);
    }
    return rhs;
}

