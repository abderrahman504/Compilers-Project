#include "Automata.h"




Automata::Automata(State* initial_state){
	this->initial_state = initial_state;
	current_states = vector<State*>();
	current_states.push_back(initial_state);
	last_acceptor_idx = -1;
	acceptor_encountered = false;
	input_idx = last_acceptor_idx + 1;
	input_stream = nullptr;
}

Automata::~Automata(){
	if(input_stream != nullptr) delete[] input_stream;
}


void Automata::reset(){
	current_states.clear();
	current_states.push_back(initial_state);
	input_idx = last_acceptor_idx + 1;
}


void Automata::setInput(char* input_stream, int size){
	if(input_stream != nullptr) delete[] input_stream;
	this->input_stream = new char[size];
	this->input_size = size;
	this->input_idx = 0;
	for (int i = 0; i < size; i++){
		this->input_stream[i] = input_stream[i];
	}
}


string Automata::nextToken(){
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


State* Automata::getLastAcceptor(){
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


int Automata::getLastTokenEnd(){
	return last_acceptor_idx;
}


bool Automata::isFinished(){
	return input_idx == input_size;
}
State* Automata::getInitialState() const { return initial_state; }