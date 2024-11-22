#include "State.h"


State::State(string name, bool is_acceptor, int acceptor_priority){
	this->name = name;
	this->is_acceptor = is_acceptor;
	this->acceptor_priority = acceptor_priority;
	transitions = unordered_map<char, vector<State*>*>();
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

void State::addEpsilonTransition(vector<State*>* next_states){
	// If there isn't an epsilon transition in the map, add it
	if(transitions.find(0) == transitions.end()){
		transitions.insert({0, new vector<State*>});
	}
	// Add each state to transitions map
	for (auto next_stat : *next_states){
		transitions[0]->push_back(next_stat);
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


unordered_map<char, vector<State*>*> State::getTransitions(){
	return transitions;
}