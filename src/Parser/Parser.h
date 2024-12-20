#include "ParsingTable.h"
#include "Lexer.h"
#include <stack>
class Parser {
private:
    ParsingTable parsingTable;
    Grammar grammar;
    Lexer lexer;
    std::stack<std::string> parseStack;

public:
    Parser(const Grammar &g, const ParsingTable &pt, const Lexer &lx)
        : grammar(g), parsingTable(pt), lexer(lx) {}
    void parse();
    void panicModeRecovery();
};
