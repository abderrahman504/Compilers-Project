#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "Grammar.h"
#include <map>

class ParsingTable
{
private:
    // maps each row x col (pair<string,string>) to entry (string)
    std::map<std::string, std::map<std::string, std::string>> table;

public:
    void constructTable(const Grammar &grammar);
    std::string getAction(const std::string &nonTerminal, const std::string &terminal) const;
    void printTable() const;
};

#endif