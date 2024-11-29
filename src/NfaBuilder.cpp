#include "NFABuilder.h"
#include "Automata.h"
#include <stack>
#include <unordered_map>

#include "NFABuilder.h"
#include <vector>
#include <iostream>

NFABuilder::NFABuilder(const FileParser& parser) : stateCounter(0),  fullNFA(createState()) {
    std::cout << "\nInside the Builder Constructor";
    std::vector<Automata> nfas;
    std::cout << "\nCreated nfas stack\n";

    // Convert regular expressions to NFAs
    const auto& regularExpressions = parser.getRegularExpressions();

    for (const auto& [name, regex] : regularExpressions) {
        std::cout <<name <<" : " << regex ;
        Automata nfa = regexToNFA(regex);
        nfas.push_back(nfa);
    }
    std::cout << "Converted Regular Expressions to NFA";

    // Convert keywords into NFAs
    const auto& keywords = parser.getKeywords();
    for (const auto& keyword : keywords) {
        Automata keywordNFA = regexToNFA(keyword);
        nfas.push_back(keywordNFA);
    }
    std::cout << "Converted Keywords to NFA";

    // Convert punctuations into NFAs
    const auto& punctuations = parser.getPunctuations();
    for (const auto& punctuation : punctuations) {
        Automata punctuationNFA = regexToNFA(punctuation);
        nfas.push_back(punctuationNFA);
    }
    std::cout << "Converted Punctuations to NFA";

    // Combine all NFAs into a single NFA
    fullNFA = combineNFAs(nfas);
}


State* NFABuilder::createState(bool isAcceptor, int acceptorPriority) {
    std::string stateName = "q" + std::to_string(stateCounter++);
    return new State(stateName, isAcceptor, acceptorPriority);
}

Automata NFABuilder::buildNFA(const std::string& regex) {
    return regexToNFA(regex);
}

Automata NFABuilder::combineNFAs(const std::vector<Automata>& nfas) {
    State* startState = createState();
    Automata combined(startState);

    for (const auto& nfa : nfas) {
        startState->addTransition('\0', std::vector<State*>({nfa.getInitialState()}));
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
    bool isEscaped = false;  // Track whether the current character is escaped
    std::cout <<"\n"<<postfix<<"\n";
    for (size_t i = 0; i < postfix.size(); ++i) {
        char c = postfix[i];

        if (isEscaped) {
            // Handle escaped symbols (e.g., \*, \+)
            if (c == '*') {
                // Literal multiplication (*)
                State* start = createState();
                State* accept = createState(true);
                start->addTransition('*', {accept});

                Automata singleCharNFA(start);
                singleCharNFA.setLastAcceptor(accept);

                nfaStack.push(singleCharNFA);
                std::cout << "Created NFA for MulOP";
            } else if (c == '+') {
                // Literal addition (+)
                State* start = createState();
                State* accept = createState(true);
                start->addTransition('+', {accept});

                Automata singleCharNFA(start);
                singleCharNFA.setLastAcceptor(accept);

                nfaStack.push(singleCharNFA);
            } else if (c == '=' && i + 2 < postfix.size() && postfix[i + 2] == '=') {
                // Handle \=\= (equals)
                i+=2; // Skip next '\='
                State* start = createState();
                State* intermediate = createState();
                State* accept = createState(true);
                start->addTransition('=', {intermediate});  // Treat '==' as a literal
                intermediate->addTransition('=',{accept});
                Automata singleCharNFA(start);
                singleCharNFA.setLastAcceptor(accept);

                nfaStack.push(singleCharNFA);
                std::cout << "Created NFA for RelOp ==\n"; 
            } else {
                throw std::runtime_error(std::string("Unknown escaped character: \\") + c);
            }
            std::cout<<"\nReset Escape Flag\n";
            isEscaped = false;  // Reset escape flag

        }else if (c == '!' && i + 2 < postfix.size() && postfix[i + 2] == '=') {
            // Handle !\= (not equals)
            i+=2; // Skip next '\='
            State* start = createState();
            State* intermediate = createState();
            State* accept = createState(true);
            start->addTransition('!', {intermediate});  // Treat '!=' as a literal
            intermediate->addTransition('=',{accept});

            Automata singleCharNFA(start);
            singleCharNFA.setLastAcceptor(accept);

            nfaStack.push(singleCharNFA);
            std::cout << "Created NFA for RelOp !=\n"; 
            }  else if (c == '>' && i + 2 < postfix.size() && postfix[i + 2] == '=') {
            // Handle >\= (greater than or equal)
            i+=2; // Skip next '\='
            State* start = createState();
            State* intermediate = createState();
            State* accept = createState(true);
            start->addTransition('>', {intermediate});  // Treat '>=' as a literal
            intermediate->addTransition('=',{accept});

            Automata singleCharNFA(start);
            singleCharNFA.setLastAcceptor(accept);

            nfaStack.push(singleCharNFA);
            std::cout << "Created NFA for RelOp >=\n";
            } else if (c == '<' && i + 2 < postfix.size() && postfix[i + 2] == '=') {
            // Handle <\= (less than or equal)
            i+=2; // Skip next '\='
            State* start = createState();
            State* intermediate = createState();
            State* accept = createState(true);
            start->addTransition('<', {intermediate});  // Treat '<=' as a literal
            intermediate->addTransition('=',{accept});
            Automata singleCharNFA(start);
            singleCharNFA.setLastAcceptor(accept);

            nfaStack.push(singleCharNFA);
            std::cout << "Created NFA for RelOp <=\n"; // Debug output
        } 
        else if (c == '\\') {
            // Handle escape character
            isEscaped = true;
            continue;
        } else if (isOperator(c)) {
            std::cout<<"\n" << c;
            // Handle regex operators
            if (c == '|') {
                if (nfaStack.size() < 2) {
                    throw std::runtime_error("Malformed regex: '|' requires two operands.");
                }
                Automata nfa1 = nfaStack.top(); nfaStack.pop();
                Automata nfa2 = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                State* accept = createState(true);
                start->addEpsilonTransition({nfa1.getInitialState(), nfa2.getInitialState()});
                nfa1.getLastAcceptor()->addEpsilonTransition({accept});
                nfa2.getLastAcceptor()->addEpsilonTransition({accept});
                Automata result = Automata(start);
                nfaStack.push(result);
            } else if (c == '*') {
                // Kleene closure
                if (nfaStack.empty()) {
                    throw std::runtime_error("Malformed regex: '*' requires one operand.");
                }
                Automata nfa = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                State* accept = createState(true);

                start->addEpsilonTransition({nfa.getInitialState(), accept});
                nfa.getLastAcceptor()->addEpsilonTransition({nfa.getInitialState(), accept});

                Automata result(start);
                result.setLastAcceptor({accept});
                nfaStack.push(result);
            } else if (c == '+') {
                // Positive closure
                if (nfaStack.empty()) {
                    throw std::runtime_error("Malformed regex: '+' requires one operand.");
                }
                // std::cout << nfaStack.size();
                Automata nfa = nfaStack.top(); nfaStack.pop();

                State* start = createState();
                State* accept = createState(true);

                start->addEpsilonTransition({nfa.getInitialState()});
                std::cout<<"before last acceptor";
                State* beforeLastAcceptor = nfa.getLastAcceptor();
                State* nfaInitialSate = nfa.getInitialState();
                beforeLastAcceptor->addEpsilonTransition({nfaInitialSate,accept});
                std::cout<<"after last acceptor";

                Automata result(start);
                nfaStack.push(result);
                std::cout << "\nOne or More\n";
            }
        } else {
            // Handle literals (non-operator characters)
            std::cout<<"\n"<<c;
            State* start = createState();
            State* accept = createState(true);
            start->addTransition(c, {accept});

            Automata singleCharNFA(start);
            singleCharNFA.setLastAcceptor(accept);

            nfaStack.push(singleCharNFA);

        }
    }

    if (nfaStack.size() != 1) {
        throw std::runtime_error("Malformed regex: too many operands.");
    }

    return nfaStack.top();
}


std::string NFABuilder::toPostfix(const std::string& regex) {
    std::stack<char> operatorStack;
    std::string postfix;
    bool escapeNext = false;

    // Helper to determine if a character is an operand
    auto isOperand = [](char c) {
        return std::isalnum(c) || c == '/' || c == '-' || c == '=' || c == '>' || c =='<' || c == '!'; // Alphanumeric characters are operands
    };

    // Helper to define precedence of operators
    auto precedence = [](char op) {
        switch (op) {
            case '*': return 3; // Kleene star
            case '+': return 3; // Positive closure
            case '.': return 2; // Concatenation
            case '|': return 1; // Alternation
            default: return 0;
        }
    };

    // Helper to check if a character is an operator
    auto isOperator = [](char c) {
        return c == '*' || c == '+' || c == '.' || c == '|';
    };

    for (size_t i = 0; i < regex.size(); ++i) {
        char c = regex[i];

        if (escapeNext) {
            // Treat escaped characters (\+ or \*) as literals
            postfix += c;
            escapeNext = false;
            continue;
        }

        if (c == '\\') {
            escapeNext = true; // Escape the next character
            postfix += c;
        } else if (isOperand(c)) {
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
const Automata& NFABuilder::getFullNFA(){ return fullNFA;}
