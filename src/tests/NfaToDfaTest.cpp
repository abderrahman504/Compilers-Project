#include "../Automata.h"
#include "../State.h"
#include "../DFAConstructor.h"
#include "../RulesParsing/FileParser.h"
#include "../NfaBuilder.h"
#include <iostream>

void nfa_to_dfa_test()
{
	// NFA is from Q1-b from sheet 2
	// It's not possible to have mulitple initial states so we'll make an initial state with epsi transitions to A,B
	// State* initial_state = new State("I", false, 0);
	// State* A = new State("A", false, 0);
	// State* B = new State("B", false, 0);
	// State* C = new State("C", false, 0);
	// State* D = new State("D", true, 0);

	// initial_state->addEpsilonTransition(vector<State*>{A, B});
	// A->addTransition('0', vector<State*>{B, C});
	// A->addTransition('1', vector<State*>{C, D});
	// B->addTransition('1', vector<State*>{D});
	// C->addTransition('0', vector<State*>{A, B});
	// C->addTransition('1', vector<State*>{C});
	// D->addTransition('1', vector<State*>{A, C});

	//Automata* nfa = new Automata(initial_state);
    FileParser parser;
	parser.parseFile("../example_rules.txt");
	NFABuilder nfaBuilder;
	Automata nfa = nfaBuilder.getFullNFA(parser.getRegularExpressions(), parser.getKeywords(), parser.getPunctuations());
	// Add code that builds a DFA from the NFA below
    Automata* NFA = &nfa;
    DFAConstructor dfaConstructor;
    Automata dfa = dfaConstructor.constructDFA(*NFA);
    dfa = dfaConstructor.minimizeDFA(dfa);

	// Print the DFA
    unordered_set<State*> dfaStates = dfa.getAllStates();  // Get all the DFA states
    std::cout << dfaStates.size() << " DFA states created." << std::endl;
    std::cout << "DFA States and Transitions:" << std::endl;

    for (State* state : dfaStates) {
        std::cout << "State: " << state->getName() << std::endl;
        unordered_map<char, vector<State*>> transitions = state->getTransitions();
        
        for (auto& transition : transitions) {
            char input = transition.first;
            vector<State*> nextStates = transition.second;
            
            std::cout << "  On input '" << input << "' transitions to: ";
            for (State* nextState : nextStates) {
                std::cout << nextState->getName() << " ";
            }
            std::cout << std::endl;
        }

        if (state->isAcceptor()) {
            std::cout << "  This state is an acceptor state." << std::endl;
        }
    }
    std::cout << endl;
    dfaConstructor.printTransitionTable(dfa);
}



