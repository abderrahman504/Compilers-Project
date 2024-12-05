#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include <unordered_map>

class FileParser {
private:
    std::unordered_map<std::string, std::string> regularDefinitions; // Stores LHS = RHS
    std::unordered_map<std::string, std::string> regularExpressions; // Stores LHS: RHS
    std::vector<std::string> keywords;                               // List of keywords
    std::vector<std::string> punctuations;                           // List of operators

    // Expands any character ranges like a-z, 0-9 etc, and substitutes any definition references.
    std::string expandRhs(std::string rhs);
    
    // Replaces any definitions referenced in `rhs` with the the definition's RHS.
    std::string substituteDefinitions(std::string rhs); 

public:
    FileParser() = default;

    // Parses the input file
    void parseFile(const std::string& filePath);

    // Getters for parsed data
    const std::unordered_map<std::string, std::string>& getRegularDefinitions() const;
    const std::unordered_map<std::string, std::string>& getRegularExpressions() const;
    const std::vector<std::string>& getKeywords() const;
    const std::vector<std::string>& getPunctuations() const;
};

#endif // FILEPARSER_H
