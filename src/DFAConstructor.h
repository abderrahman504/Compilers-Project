#ifndef SUBSET_CONSTRUCTION_H
#define SUBSET_CONSTRUCTION_H

#include "Automata.h"
#include <set>
#include <unordered_map>
#include <queue>

class DFAConstructor {
public:
    Automata constructDFA(const Automata& nfa);
    Automata minimizeDFA(const Automata& dfa);
private:
    std::set<State*> epsilonClosure(const std::set<State*>& states);
    bool containsAcceptor(const std::set<State*>& states);
    int highestPriority(const std::set<State*>& states);
};

#endif
