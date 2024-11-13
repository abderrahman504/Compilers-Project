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
	void reset(){
		current_states.clear();
		current_states.push_back(initial_state);
		input_idx = last_acceptor_idx + 1;
	}

	public:

	Automata(State* initial_state){
		this->initial_state = initial_state;
		current_states = vector<State*>();
		current_states.push_back(initial_state);
		last_acceptor_idx = -1;
		acceptor_encountered = false;
		input_idx = last_acceptor_idx + 1;
		input_stream = nullptr;
	}
	
	~Automata(){
		if(input_stream != nullptr) delete[] input_stream;
	}

	// Sets the input that this automata will read from
	void setInput(char* input_stream, int size){
		if(input_stream != nullptr) delete[] input_stream;
		this->input_stream = new char[size];
		this->input_size = size;
		this->input_idx = 0;
		for (int i = 0; i < size; i++){
			this->input_stream[i] = input_stream[i];
		}
	}

	// Reads and returns the next valid token from the input stream. If no valid token is found then the remaining input is returned as an error token.
	// Use getLastAcceptor() to determine the type of the token returned.
	string nextToken(){
		acceptor_encountered = false;

		vector<char> chars = vector<char>();
		int local_acceptor_idx = -1;

		// Keep processing input until the eof or null state is reached
		// Null state is represented by an empty current_states vector
		while(input_idx < input_size && current_states.size() != 0){
			char c = input_stream[input_idx];
			vector<State*> next_states = vector<State*>();
			// Transition from current states to next states using c as input
			for (auto state : current_states){
				vector<State*>* next = state->getTransitions(c);
				// If transition exists then add states to next_states
				if(next != nullptr){
					for (auto next_state : *next){
						if(!next_state->isNull()) next_states.push_back(next_state);
					}
				}
			}
			// Check if next_states includes acceptors and update last_acceptor and last_acceptor_idx
			for (auto state : next_states){
				if(state->isAcceptor()){
					acceptor_encountered = true;
					last_acceptor = next_states;
					last_acceptor_idx = input_idx;
					local_acceptor_idx = chars.size();
					break;
				}
			}

			chars.push_back(c);
			input_idx++;
			current_states = next_states;
		}
		
		// Create a token out of chars
		auto token_end = acceptor_encountered ? chars.begin()+local_acceptor_idx+1 : chars.end(); 
		string token = string(chars.begin(), token_end);
		reset();
		return token;

	}

	// Returns the latest acceptor state with highest priority this automata encountered while reading input.
	State* getLastAcceptor(){
		if(!acceptor_encountered) return nullptr;

		State* acceptor = nullptr;
		int priority = 0;
		for (auto state : last_acceptor){
			if(state->isAcceptor()){
				if(acceptor == nullptr || state->getAcceptorPriority() > priority){
					acceptor = state;
					priority = state->getAcceptorPriority();
				}
			}
		}
		return acceptor;
	}

	// Returns the index of the last character of the last accepted token in the input.
	int getLastTokenEnd(){
		return last_acceptor_idx;
	}


	bool isFinished(){
		return input_idx == input_size;
	}

};