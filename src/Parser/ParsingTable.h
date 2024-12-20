#include "Grammar.h"
class ParsingTable {
private:
    //maps each row x col (pair<string,string>) to entry (string)
    std::unordered_map<std::pair<std::string, std::string>, std::string> table;

public:
    void constructTable(const Grammar &grammar);
    std::string getAction(const std::string &nonTerminal, const std::string &terminal) const;
    void printTable() const;
};
