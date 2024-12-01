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
            regularDefinitions[match[1]] = expandRhs(match[2].str());
        } else if (std::regex_match(line, match, exprRegex)) {
            // Regular Expression
            regularExpressions[match[1]] = expandRhs(match[2]);
        } else if (std::regex_match(line, match, keywordRegex)) {
            // Keywords
            stringstream ss(match[1]);
            string keyword;
            while (std::getline(ss, keyword, ' ')) {
                keywords.push_back(keyword);
            }
        } else if (std::regex_match(line, match, punctuationRegex)) {
            // Punctuation
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
const std::unordered_map<string, string>& FileParser::getRegularDefinitions() const {
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


string FileParser::expandRhs(string rhs)
{
    // if rhs is referencing any other definitions then substitute them
    rhs = substituteDefinitions(rhs);

    //string expanded_rhs = "";
    //int idx = -1;
    //while (++idx < rhs.size())
    //{
    //    char c = rhs[idx];
    //     If c is followed by a dash and another character then this is a range of characters
    //    if(idx < rhs.size()-2 && rhs[idx+1] == '-'){
    //        expanded_rhs += '(';
    //        for(char i = c; i <= rhs[idx+2]; i++){
    //            expanded_rhs += i;
    //            expanded_rhs += '|';
    //        }
    //        expanded_rhs.pop_back();
    //        expanded_rhs += ')';
    //        idx += 2;
    //        continue;
    //    }
    //    else{ //Otherwise add the character to the rhs
    //        expanded_rhs += c;
    //    }
    //}

    return rhs;
}

string FileParser::substituteDefinitions(string rhs)
{
    for (auto entry : regularDefinitions){
        int start = 0;
        while(true){
            int pos = rhs.find(entry.first, start);
            if(pos == string::npos) break;
            rhs.replace(pos, entry.first.size(), '(' + entry.second + ')');
            start = pos + entry.first.size();
        }
    }
    return rhs;
}


//string FileParser::processExpressionRhs(string rhs){
//    unordered_map<string, string> substitutions = unordered_map<string, string>();
//    for (auto it : regularDefinitions){
//        // Check if it.first exists in rhs and replace it with the strings in it.second piped up
//        string def_name = it.first;
//        vector<string> def_values = it.second;
//        string def_values_piped = "(";
//        for (auto val : def_values){
//            def_values_piped += val + "|";
//        }
//        def_values_piped.pop_back(); // Remove the last pipe
//        def_values_piped += ")";
//        substitutions[def_name] = def_values_piped;
//    }

//    // Replace all definitions in the rhs
//    for (auto it : substitutions){
//        string def_name = it.first;
//        string def_values_piped = it.second;
//        rhs = regex_replace(rhs, regex(def_name), def_values_piped);
//    }
//    return rhs;
//}

