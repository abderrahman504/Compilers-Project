#ifndef NFABUILDER_H
#define NFABUILDER_H

#include <string>
#include <vector>
#include "Automata.h" // Automata class
#include "State.h"    // State class

class NFABuilder {
public:
    NFABuilder();

    // Builds an NFA for a single regular expression
    Automata buildNFA(const std::string& regex);

    // Combines multiple NFAs into one
    Automata combineNFAs(const std::vector<Automata>& nfas);

private:
    int stateCounter; // Counter to assign unique IDs to states

    // Helper methods for NFA construction
    State* createState(bool isAcceptor = false, int acceptorPriority = 0);
    std::string toPostfix(const std::string& regex); // Convert infix to postfix
    bool isOperator(char c);
    int precedence(char op);

    // Automata operations
    Automata regexToNFA(const std::string& regex);
};

#endif // NFABUILDER_H
