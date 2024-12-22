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
    const string epsilon = "Epsilon";

    std::vector<std::string> nonTerminals;
    std::vector<std::string> terminals;
    std::unordered_map<std::string, std::vector<Production>> productions;
    std::unordered_map<std::string, std::unordered_set<std::string>> firstSet;
    std::unordered_map<std::string, std::unordered_set<std::string>> followSet;

public:
    void loadFromFile(const std::string &filename);
    void computeFirstSet();
    void computeFollowSet();
    bool isLL1();
    const std::unordered_map<std::string, std::vector<Production>>& getProductions() const;
    const std::unordered_map<std::string, std::unordered_set<std::string>> getFirstSet();
    const std::unordered_map<std::string, std::unordered_set<std::string>> getFollowSet();
};
