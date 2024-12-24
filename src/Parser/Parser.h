#ifndef PARSER_H
#define PARSER_H

#include "ParsingTable.h"
#include "../lexical_analysis/ProgramAnalyzer.h"
#include <stack>
class Parser {
private:
    ParsingTable parsingTable;
    Grammar grmr;
    std::stack<std::string> parseStack;
    string eof;
    string epsilon;
    string start_symbol;

public:

    Parser(string grammar_file);
    void parse(vector<TableEntry> input_tokens);
};

#endif