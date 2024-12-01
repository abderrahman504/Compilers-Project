#ifndef AUTOMATA_H
#define AUTOMATA_H
#include "State.h"
#include <vector>
#include <unordered_set>

using namespace std;


/*
How is this class used:

automata = Automata(initial_state);
vector<string> tokens;
vector<State*> acceptors;
automata.setInput(line, line_size);
int line_progress = 0;
while(!automata.isFinished()){
	string token = automata.nextToken();
	if(token.getLastAcceptor() == nullptr){
		// lexical error in the remaining input
		// Throw error for invalid token
		break;
	}
	else{
		line_progress += token.size();
		tokens.push_back(token);
		acceptors.push_back(automata.getLastAcceptor());
	}
}

// we now have the tokens and corresponding acceptor states. 
// From the states we can determine the type of Regular expression each token belongs to.

*/

class Automata
{
	
	private:

	State* initial_state; // The initial state of the automata.
	unordered_set<State*> current_states; // The current state(s) this automata is in.


	public:

	Automata() = default;
	Automata(State* initial_state);
	
	~Automata();
	
	// Resets the automata to the initial state.
	void reset();

	// Feeds a single character to the automata.
	void consume(char c);

	// Returns the current states this automata is in.
	unordered_set<State*> getCurrentState();

/**
 * @return Automata's initial state.
 * 
 */
    State *getInitialState() const;

/**
 * @brief merge 2 NFAs together into a single automata
 * 
 * @param other the other NFA to be merged with
 */
	void merge(const Automata &other);

	// Returns all states in the automata
	unordered_set<State*> getAllStates() const;
};

#endif