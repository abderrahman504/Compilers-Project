#ifndef NFABUILDER_H
#define NFABUILDER_H

#include "Automata.h"
#include "FileParser.h"

class NFABuilder {
private:
    Automata fullNFA;
    int stateCounter;

    State* createState(bool isAcceptor = false, int acceptorPriority = -1);
    Automata regexToNFA(const std::string& regex);
    Automata combineNFAs(const std::vector<Automata>& nfas);

public:
    NFABuilder(const FileParser& parser);

    // Getter for the built NFA
    const Automata& getFullNFA() const { return fullNFA; }
};

#endif // NFABUILDER_H
