#ifndef STATE_H
#define STATE_H
#include <string>
#include <unordered_map>
#include <vector>


using namespace std;

class State
{
	
	private:

	string name;
	bool is_acceptor;
	int acceptor_priority;
	unordered_map<char, vector<State*>> transitions;


	public:

	State(string name, bool is_acceptor, int acceptor_priority);
	State(const State& s);

	~State();

	// Adds a number of transitions from this state to `next_states` when `input` is provided
	void addTransition(char input, vector<State*> next_states);

	// Adds a number of transitions from this state to `next_states` when no input is provided
	void addEpsilonTransition(vector<State*> next_states);

	// Gets all states that this state moves to when `input` is provided
	vector<State*> getTransitions(char input);

	//Gets all states that are moved to with a epsilon_transition
	vector<State*> getEpsilonTransitions();

	// Returns true if this state is an acceptor state
	bool isAcceptor();

	// Returns the acceptor priority of this state
	int getAcceptorPriority();

	// Returns all transitions from this state
	unordered_map<char, vector<State*>> getTransitions();

	// Returns the name of this state
	string getName();

};

#endif