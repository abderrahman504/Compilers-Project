#include "Automata.h"
#include "State.h"


void nfa_to_dfa_test()
{
	// NFA is from Q1-b from sheet 2
	// It's not possible to have mulitple initial states so we'll make an initial state with epsi transitions to A,B
	State* initial_state = new State("I", false, 0);
	State* A = new State("A", false, 0);
	State* B = new State("B", false, 0);
	State* C = new State("C", false, 0);
	State* D = new State("D", true, 0);

	initial_state->addEpsilonTransition(vector<State*>{A, B});
	A->addTransition('0', vector<State*>{B, C});
	A->addTransition('1', vector<State*>{C, D});
	B->addTransition('1', vector<State*>{D});
	C->addTransition('0', vector<State*>{A, B});
	C->addTransition('1', vector<State*>{C});
	D->addTransition('1', vector<State*>{A, C});

	Automata* nfa = new Automata(initial_state);

	// Add code that builds a DFA from the NFA below

}

