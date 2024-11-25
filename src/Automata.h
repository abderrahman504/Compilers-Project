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
	unordered_set<State*> last_acceptor; // Stores the last acceptor state(s) encountered since the last call to nextToken().
	int last_acceptor_idx; // The index in the input stream where the last acceptor state was encountered.
	bool acceptor_encountered; // whether an acceptor state has been encountered since the last call to nextToken().
	int input_idx; // Tracks the progress of the automata through the input stream.
	char* input_stream; // The input stream to read from.
	int input_size; // Size of the input stream.


	public:

	Automata(State* initial_state);
	
	~Automata();
	
	// Resets the automata to the initial state. Progress through the input_stream is unaffected.
	void reset();

	// Sets the input that this automata will read from. 
	// Use nextToken() to tokenize the input until isFinished() is true
	void setInput(char* input_stream, int size);

	// Reads and returns the next valid token from the input stream. If no valid token is found then the remaining input is returned as an error token.
	// Use getLastAcceptor() to determine the type of the token returned.
	string nextToken();

	// Returns the latest acceptor state with highest priority this automata encountered while reading input, or NULL if an invalid token is encountered.
	State* getLastAcceptor();

	// Returns the index of the last character of the last accepted token in the input.
	int getLastTokenEnd();

	// Returns the current states this automata is in.
	unordered_set<State*> getCurrentState();

	// Returns true if all input provided by `setInput()` has been processed into tokens.
	bool isFinished();
/**
 * @return Automata's initial state.
 * 
 */
    State *getInitialState() const;
/**
 * @brief Set the Last Acceptor object
 * 
 * @param s state to be the last acceptor state.
 */
    void setLastAcceptor(State *s);
    
    /**
     * @brief Set the Initial State object
     *
     */
    void setInitialState();
/**
 * @brief merge 2 NFAs together into a single automata
 * 
 * @param other the other NFA to be merged with
 */
	void merge(const Automata &other);

	


    State *getInitialState() const;

    void merge(const Automata &other);

    void setLastAcceptor(State *acceptor);
	
};

#endif