#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Production{
    vector<string> symbols;    
    Production(vector<string> symbols) : symbols(symbols) {} // Implementation is so simple there is no need to do it in source file.
    Production() = default;
};


class Grammar {
private:
    string start_symbol;
    unordered_set<string> terminals;
    unordered_set<string> non_terminals;
    unordered_map<string, vector<Production>> productions;
    unordered_map<string, unordered_map<string, int>> firstSets; // Each element in a First set is mapped to the production that generated it.
    unordered_map<string, unordered_set<string>> followSets;
    
    // Builds FIRST(`symbol`) if it doesn't exist
    void buildFirst(string symbol);
    //// Builds FOLLOW(`symbol`) if it doesn't exist
    //void buildFollow(string symbol);
    // Returns the first set of `symbol`. `symbol` Can be a terminal or non-terminal.
    const unordered_map<string, int> getFirst(string symbol);
    // Returns the follow set of `symbol`. `symbol` must be a non-terminal.
    const unordered_set<string> getFollow(string symbol);

public:
    const string epsilon = "Epsilon";
    const string eof = "EOF";

    Grammar(string grammer_file);

    // Builds the FIRST sets of all non-terminals
    void computeFirsts();
    // Builds the FOLLOW sets of all non-terminals
    void computeFollows();

    bool isLL1();
    const unordered_map<string, vector<Production>>& getProductions() const;
    // Returns first sets of all non-terminals.
    const unordered_map<string, unordered_map<string, int>>& getFirstSets();
    // Returns follow sets of all non-terminals.
    const unordered_map<string, unordered_set<string>>& getFollowSets();
};
