#ifndef PARSER_H
#define PARSER_H

#include "ParsingTable.h"
#include <stack>
class Parser {
private:
    ParsingTable parsingTable;
    Grammar grammar;
    std::stack<std::string> parseStack;

public:
    Parser(const Grammar &g, const ParsingTable &pt) : grammar(g), parsingTable(pt) {}
    void parse();
    void panicModeRecovery();
};

#endif