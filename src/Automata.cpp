#include "Automata.h"
#include <algorithm>



Automata::Automata(State* initial_state){
	this->initial_state = initial_state;
	current_states = unordered_set<State*>();
	reset();
	last_acceptor_idx = -1;
	acceptor_encountered = false;
	input_stream = nullptr;
}

Automata::~Automata(){
	if(input_stream != nullptr) delete[] input_stream;
}


void Automata::reset(){
	current_states.clear();
	current_states.insert(initial_state);
	// If there are epsilon transitions from the initial state then add them to the current state.
	vector<State*> epsi_trans = initial_state->getEpsilonTransitions();
	for(auto state : epsi_trans){
		current_states.insert(state);
	}
}


void Automata::setInput(char* input_stream, int size){
	if(input_stream != nullptr) delete[] input_stream;
	this->input_stream = new char[size];
	this->input_size = size;
	for (int i = 0; i < size; i++){
		this->input_stream[i] = input_stream[i];
	}
	input_idx = 0;	
	acceptor_encountered = false;
	last_acceptor = unordered_set<State*>();
}


string Automata::nextToken(){
	acceptor_encountered = false;

	vector<char> chars = vector<char>();
	int local_acceptor_idx = -1;

	// Keep processing input until the eof or null state is reached
	// Null state is represented by an empty current_states set
	while(input_idx < input_size && current_states.size() != 0){
		char c = input_stream[input_idx];
		unordered_set<State*> next_states = unordered_set<State*>();
		// Transition from current states to next states using c as input
		for (auto state : current_states){
			vector<State*> next = state->getTransitions(c);
			for (auto next_state : next){
				next_states.insert(next_state);
			}
		}
		// If any epsilon transitions exist from a state in next_states then add them to next_states
		for (auto state : next_states){
			vector<State*> epsi_trans = state->getEpsilonTransitions();
			for (auto next_state : epsi_trans){
				next_states.insert(next_state);
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
	input_idx = last_acceptor_idx + 1;
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

unordered_set<State*> Automata::getCurrentState(){
	return current_states;
}



bool Automata::isFinished(){
	return input_idx == input_size;
}
State* Automata::getInitialState() const { return initial_state; }


void Automata::merge(const Automata& other) {
	// Abdelrahman's suggestion for merge implementation
	State* new_initial = new State("", false, 0);
	vector<State*> vec = vector<State*>{initial_state, other.initial_state};
	new_initial->addEpsilonTransition(vec);
	initial_state = new_initial;

}

void Automata::setLastAcceptor(State* acceptor) {
	last_acceptor = std::unordered_set<State*>{acceptor};
}


