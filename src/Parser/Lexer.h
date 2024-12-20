#include <queue>
#include <string>
class Lexer {
private:
    std::queue<std::string> tokens;

public:
    void loadSourceCode(const std::string &sourceCode);
    std::string nextToken();
};
