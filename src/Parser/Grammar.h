#include <vector>
#include <string>
#include <map>
#include <set>

class Grammar {
private:
    std::vector<std::string> nonTerminals;
    std::vector<std::string> terminals;
    std::map<std::string, std::vector<std::vector<std::string>>> productions;
    std::map<std::string, std::set<std::string>> firstSet;
    std::map<std::string, std::set<std::string>> followSet;

public:
    void loadFromFile(const std::string &filename);
    void computeFirstSet();
    void computeFollowSet();
    bool isLL1();
    const std::map<std::string, std::vector<std::vector<std::string>>>& getProductions() const;
    const std::map<std::string, std::set<std::string>> getFirstSet();
    const std::map<std::string, std::set<std::string>> getFollowSet();
};
