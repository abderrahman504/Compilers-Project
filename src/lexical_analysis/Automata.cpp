#include "Automata.h"
#include <algorithm>
#include <queue>



Automata::Automata(State* initial_state){
	this->initial_state = initial_state;
	current_states = unordered_set<State*>();
	reset();
}

Automata::~Automata(){}


void Automata::reset(){
	current_states.clear();
	current_states.insert(initial_state);
	// If there are epsilon transitions from the initial state then add them to the current state.
	bool states_added = true;
	while (states_added){
		states_added = false;
		for (auto state : current_states){
			vector<State*> epsi_trans = state->getEpsilonTransitions();
			for (auto next : epsi_trans){
				if(current_states.find(next) == current_states.end()){
					states_added = true;
					current_states.insert(next);
				}
			}
		}
	}
}

void Automata::consume(char c)
{
	unordered_set<State*> next_states = unordered_set<State*>();
	// Transition from current states to next states using c as input
	for (auto state : current_states){
		vector<State*> next = state->getTransitions(c);
		for (auto next_state : next){
			next_states.insert(next_state);
		}
	}
	// If any epsilon transitions exist from a state in next_states then add them to next_states
	bool states_added = true;
	while (states_added){
		states_added = false;
		for (auto state : next_states){
			vector<State*> epsi_trans = state->getEpsilonTransitions();
			for (auto next : epsi_trans){
				if(next_states.find(next) == next_states.end()){
					states_added = true;
					next_states.insert(next);
				}
			}
		}
	}
	current_states = next_states;
}


unordered_set<State*> Automata::getCurrentState(){
	return current_states;
}


State* Automata::getInitialState() const { return initial_state; }


unordered_set<State*> Automata::getAllStates() const{
    unordered_set<State*> visited; // To keep track of visited states
    queue<State*> to_visit;        // Queue for BFS traversal

    // Start from the initial state
    to_visit.push(initial_state);
    visited.insert(initial_state);

    while (!to_visit.empty()) {
        State* current = to_visit.front();
        to_visit.pop();

        // Get all transitions (both normal and epsilon)
        unordered_map<char, vector<State*>> transitions = current->getTransitions();
        vector<State*> epsilon_transitions = current->getEpsilonTransitions();

        // Add all reachable states
        for (const auto& [input, states] : transitions) {
            for (State* state : states) {
                if (visited.find(state) == visited.end()) {
                    visited.insert(state);
                    to_visit.push(state);
                }
            }
        }

        for (State* state : epsilon_transitions) {
            if (visited.find(state) == visited.end()) {
                visited.insert(state);
                to_visit.push(state);
            }
        }
    }

    return visited; // Return all visited states
}



