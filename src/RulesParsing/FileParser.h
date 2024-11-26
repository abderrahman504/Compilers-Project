#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include <unordered_map>

class FileParser {
private:
    std::unordered_map<std::string, std::vector<std::string>> regularDefinitions; // Stores LHS = RHS
    std::unordered_map<std::string, std::string> regularExpressions; // Stores LHS: RHS
    std::vector<std::string> keywords;                               // List of keywords
    std::vector<std::string> punctuations;                           // List of operators

    std::vector<std::string> processDefinitionRhs(std::string rhs);  // Turns the RHS of a definition into a vector of all strings recongized as the definition
    std::string processExpressionRhs(std::string rhs);  // Expands any definitions found in the RHS of an expression.

public:
    FileParser() = default;

    // Parses the input file
    void parseFile(const std::string& filePath);

    // Getters for parsed data
    const std::unordered_map<std::string, std::vector<std::string>>& getRegularDefinitions() const;
    const std::unordered_map<std::string, std::string>& getRegularExpressions() const;
    const std::vector<std::string>& getKeywords() const;
    const std::vector<std::string>& getPunctuations() const;
};

#endif // FILEPARSER_H
