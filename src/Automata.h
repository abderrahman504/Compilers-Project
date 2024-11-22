#ifndef AUTOMATA_H
#define AUTOMATA_H
#include "State.h"
#include <vector>

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
	vector<State*> current_states; // The current state(s) this automata is in.
	vector<State*> last_acceptor; // Stores the last acceptor state(s) encountered since the last call to nextToken().
	int last_acceptor_idx; // The index in the input stream where the last acceptor state was encountered.
	bool acceptor_encountered; // whether an acceptor state has been encountered since the last call to nextToken().
	int input_idx; // Tracks the progress of the automata through the input stream.
	char* input_stream; // The input stream to read from.
	int input_size; // Size of the input stream.

	// Resets the automata to the initial state. Progress through the input_stream is unaffected.
	void reset();

	public:

	Automata(State* initial_state);
	
	~Automata();

	// Sets the input that this automata will read from
	void setInput(char* input_stream, int size);

	// Reads and returns the next valid token from the input stream. If no valid token is found then the remaining input is returned as an error token.
	// Use getLastAcceptor() to determine the type of the token returned.
	string nextToken();

	// Returns the latest acceptor state with highest priority this automata encountered while reading input.
	State* getLastAcceptor();

	// Returns the index of the last character of the last accepted token in the input.
	int getLastTokenEnd();


	bool isFinished();

    State *getInitialState() const;

    void merge(const Automata &other);

    void setLastAcceptor(State *acceptor);
	
};

#endif