#include <iostream>
#include "RulesParsing/FileParser.h"
#include "NFABuilder.h"
#include "Automata.h"

void printNFA(const Automata& nfa) {
    // Traverse the NFA and print states and transitions (for testing purposes)
    std::unordered_set<State*> visited;
    std::vector<State*> toVisit = { nfa.getInitialState() };

    while (!toVisit.empty()) {
        State* current = toVisit.back();
        toVisit.pop_back();

        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);

        std::cout << "State: " << current->getName() << (current->isAcceptor() ? " (Acceptor)" : "") << "\n";

        for (char c = ' '; c <= '~'; ++c) { // Check all printable ASCII characters
            std::vector<State*> transitions = current->getTransitions(c);
            for (State* nextState : transitions) {
                std::cout << "  --[" << c << "]--> " << nextState->getName() << "\n";
                toVisit.push_back(nextState);
            }
        }

        auto epsilonTransitions = current->getEpsilonTransitions();
        for (State* nextState : epsilonTransitions) {
            std::cout << "  --[ε]--> " << nextState->getName() << "\n";
            toVisit.push_back(nextState);
        }
    }
}

int main() {
    try {
        // Step 1: Parse the file
        FileParser parser;
        parser.parseFile("D:/College/4th Year/1st Semester/Introduction to Compilers/Compilers-Project/example_rules.txt");
        std::cout << "Read Rules File Successfully";

        // Step 2: Build the NFA
        NFABuilder builder(parser);
        const Automata& nfa = builder.getFullNFA();
        std::cout << "Built NFA successfuly";
        // Step 3: Print the NFA structure
        std::cout << "Generated NFA:\n";
        printNFA(nfa);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}
