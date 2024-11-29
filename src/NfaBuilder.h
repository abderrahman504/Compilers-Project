#ifndef NFABUILDER_H
#define NFABUILDER_H

#include "Automata.h"
#include "RulesParsing/FileParser.h"

class NFABuilder {
private:
    Automata fullNFA;
    int stateCounter;

    State* createState(bool isAcceptor = false, int acceptorPriority = -1);
    Automata buildNFA(const std::string &regex);
    Automata regexToNFA(const std::string &regex);
    std::string toPostfix(const std::string &regex);
    bool isOperator(char c);
    int precedence(char op);
    Automata combineNFAs(const std::vector<Automata> &nfas);

public:
    NFABuilder(const FileParser& parser);

    // Getter for the built NFA
    const Automata& getFullNFA();
};

#endif // NFABUILDER_H
