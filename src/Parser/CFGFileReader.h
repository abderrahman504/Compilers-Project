#ifndef CFGFILEREADER_H
#define CFGFILEREADER_H

#include <string>
#include <vector>
#include <unordered_map>

class CFGFileReader {
public:
    // Define a type alias for storing the production rules
    using RulesMap = std::unordered_map<std::string, std::vector<std::string>>;

    // Method to read and parse the CFG file
    RulesMap readCFG(const std::string& filename);

private:
    // Utility method to trim whitespace from a string
    std::string trim(const std::string& str);

    // Method to parse and store a single production rule
    void parseAndStoreRule(const std::string& lhs, const std::string& rhs, RulesMap& rules);
};

#endif // CFGFILEREADER_H
