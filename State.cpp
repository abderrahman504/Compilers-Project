#include "State.h"


State::State(string name, bool is_acceptor, int acceptor_priority, bool is_null){
	this->name = name;
	this->is_acceptor = is_acceptor;
	this->acceptor_priority = acceptor_priority;
	this->is_null = is_null;
	transitions = unordered_map<char, vector<State*>*>();
}
    // copy constructor
State::State(const State& s)
{
	name = s.name;
	is_acceptor = s.is_acceptor;
	acceptor_priority = s.acceptor_priority;
	is_null = s.is_null;
	transitions = unordered_map<char, vector<State*>*>();
	transitions = s.transitions;
}
State::~State(){
	// delete each vector in the transitions map
	for (auto it = transitions.begin(); it != transitions.end(); ++it){
		delete it->second;
	}
}

void State::addTransition(char input, vector<State*>* next_states){
	// If this input isn't in the transitions map, add it
	if(transitions.find(input) == transitions.end()){
		transitions.insert({input, new vector<State*>});
	}
	// Add each state in next_states to transitions map
	for (auto next_stat : *next_states){
		transitions[input]->push_back(next_stat);
	}
}

vector<State*>* State::getTransitions(char input){
	if(transitions.find(input) == transitions.end()){
		return nullptr;
	}
	else return transitions[input];
}

bool State::isAcceptor(){return is_acceptor;}

int State::getAcceptorPriority(){return acceptor_priority;}

bool State::isNull(){return is_null;}