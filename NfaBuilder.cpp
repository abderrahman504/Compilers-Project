#include "NFABuilder.h"
#include "Automata.h"
#include <stack>
#include <unordered_map>

NFABuilder::NFABuilder() : stateCounter(0) {}

State* NFABuilder::createState(bool isAcceptor, int acceptorPriority) {
    std::string stateName = "q" + std::to_string(stateCounter++);
    return new State(stateName, isAcceptor, acceptorPriority, false);
}

Automata NFABuilder::buildNFA(const std::string& regex) {
    return regexToNFA(regex);
}

Automata NFABuilder::combineNFAs(const std::vector<Automata>& nfas) {
    State* startState = createState();
    Automata combined(startState);

    for (const auto& nfa : nfas) {
        startState->addTransition('\0', new std::vector<State*>({nfa.getInitialState()}));
    }

    return combined;
}

Automata NFABuilder::regexToNFA(const std::string& regex) {
    std::stack<Automata> nfaStack;
    std::string postfix = toPostfix(regex);

    for (char c : postfix) {
        if (isOperator(c)) {
            if (c == '|') {
                Automata nfa2 = nfaStack.top(); nfaStack.pop();
                Automata nfa1 = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                start->addTransition('\0', new std::vector<State*>({nfa1.getInitialState()}));
                start->addTransition('\0', new std::vector<State*>({nfa2.getInitialState()}));

                Automata result(start);
                result.merge(nfa1);
                result.merge(nfa2);

                nfaStack.push(result);
            } else if (c == '*') {
                Automata nfa = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                State* accept = createState(true);

                start->addTransition('\0', new std::vector<State*>({nfa.getInitialState(), accept}));
                nfa.last_acceptor->addTransition('\0', new std::vector<State*>({nfa.getInitialState(), accept}));

                Automata result(start);
                result.merge(nfa);

                nfaStack.push(result);
            }
        } else {
            State* start = createState();
            State* accept = createState(true);
            start->addTransition(c, new std::vector<State*>({accept}));

            Automata singleCharNFA(start);
            singleCharNFA.last_acceptor = accept;

            nfaStack.push(singleCharNFA);
        }
    }

    return nfaStack.top();
}

std::string NFABuilder::toPostfix(const std::string& regex) {
    // Implement Shunting-Yard algorithm to convert regex from infix to postfix
    return ""; // Placeholder
}

bool NFABuilder::isOperator(char c) {
    return c == '|' || c == '*' || c == '+' || c == '(' || c == ')';
}

int NFABuilder::precedence(char op) {
    if (op == '*') return 3;
    if (op == '|') return 1;
    return 0;
}
