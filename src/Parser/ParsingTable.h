#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "Grammar.h"
#include <unordered_map>

class ParsingTable
{
private:
    // maps each row x col (pair<string,string>) to entry (string)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> table;
    vector<string> terminals;
    vector<string> non_terminals;

public:

    const string synch = "SYNCH";
    const string error = "ERROR";

    void constructTable(const Grammar &grammar);
    std::string getAction(const std::string &nonTerminal, const std::string &terminal) const;
    void printTable() const;
};

#endif