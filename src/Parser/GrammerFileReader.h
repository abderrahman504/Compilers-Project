#ifndef GRAMMER_FILE_READER_H
#define GRAMMER_FILE_READER_H
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class GrammerFileReader {

    string start_symbol;
    unordered_set<string> terminals;
    unordered_set<string> non_terminals;
    unordered_map<string, vector<vector<string>>> productions;

    public:

    const string epsilon = "Epsilon";

    void readRules(string file_path);
    string getStartSymbol();
    unordered_set<string> getTerminals();
    unordered_set<string> getNonTerminals();
    unordered_map<string, vector<vector<string>>> getProductions();
};

#endif
