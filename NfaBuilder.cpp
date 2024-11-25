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
/**
 * @brief Builds a Non-Deterministic Finite Automaton (NFA) from a regular expression using Thomson's Construction.
 * 
 * @param regex The regular expression to convert into an NFA.
 * @return Automata The constructed NFA representing the given regular expression.
 */
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
                // No need for merge, try it.
                // result.merge(nfa1);
                // result.merge(nfa2);

                nfaStack.push(result);
            } else if (c == '*') {
                Automata nfa = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                State* accept = createState(true);

                start->addTransition('\0', new std::vector<State*>({nfa.getInitialState(), accept}));
                nfa.getLastAcceptor()->addTransition('\0', new std::vector<State*>({nfa.getInitialState(), accept}));

                Automata result(start);
                // No need for merge, try it.
                // result.merge(nfa);

                nfaStack.push(result);
            }
        } else {
            State* start = createState();
            State* accept = createState(true);
            start->addTransition(c, new std::vector<State*>({accept}));

            Automata singleCharNFA(start);
            singleCharNFA.setLastAcceptor(accept);

            nfaStack.push(singleCharNFA);
        }
    }

    return nfaStack.top();
}

std::string NFABuilder::toPostfix(const std::string& regex) {
    std::stack<char> operatorStack;
    std::string postfix;

    auto isOperand = [](char c) {
        return std::isalnum(c); // Alphanumeric characters are operands
    };

    auto precedence = [](char op) {
        switch (op) {
            case '*': return 3; // High precedence for Kleene star
            case '.': return 2; // Concatenation (implicit)
            case '|': return 1; // Alternation
            default: return 0;
        }
    };

    auto isOperator = [](char c) {
        return c == '*' || c == '.' || c == '|';
    };

    for (char c : regex) {
        if (isOperand(c)) {
            postfix += c; // Append operands directly to the postfix string
        } else if (c == '(') {
            operatorStack.push(c); // Push '(' to the stack
        } else if (c == ')') {
            // Pop until '(' is encountered
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            if (!operatorStack.empty() && operatorStack.top() == '(') {
                operatorStack.pop(); // Remove the '('
            }
        } else if (isOperator(c)) {
            // Pop operators of higher or equal precedence
            while (!operatorStack.empty() &&
                   precedence(operatorStack.top()) >= precedence(c)) {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(c); // Push the current operator
        }
    }

    // Pop remaining operators
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return postfix;
}


bool NFABuilder::isOperator(char c) {
    return c == '|' || c == '*' || c == '+' || c == '(' || c == ')';
}

int NFABuilder::precedence(char op) {
    if (op == '*') return 3;
    if (op == '|') return 1;
    return 0;
}
