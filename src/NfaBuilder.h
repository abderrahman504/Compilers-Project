#ifndef NFABUILDER_H
#define NFABUILDER_H

#include "Automata.h"
#include "RulesParsing/FileParser.h"

class NFABuilder {
private:
    int stateCounter;

    State* createState(bool isAcceptor = false, int acceptorPriority = -1);
    void regexToNFA(const std::string &regex, const std::string &name, int priority, State** out_initial, State** out_acceptor);

public:
    NFABuilder();

    // Constructs an NFA out of the expressions, keywords, and punctuations
    Automata getFullNFA(std::unordered_map<std::string, std::string> expressions, std::vector<std::string> keywords, std::vector<std::string> punctuations);
};

#endif // NFABUILDER_H
