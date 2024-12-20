#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Grammar {
private:
    std::vector<std::string> nonTerminals;
    std::vector<std::string> terminals;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> productions;
    std::unordered_map<std::string, std::unordered_set<std::string>> firstSet;
    std::unordered_map<std::string, std::unordered_set<std::string>> followSet;

public:
    void loadFromFile(const std::string &filename);
    void computeFirstSet();
    void computeFollowSet();
    bool isLL1();
    const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& getProductions() const;
    const std::unordered_map<std::string, std::unordered_set<std::string>> getFirstSet();
    const std::unordered_map<std::string, std::unordered_set<std::string>> getFollowSet();
};
