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
	unordered_map<char, vector<State*>*> transitions;
	bool is_null;


	public:

	State(string name, bool is_acceptor, int acceptor_priority, bool is_null);

	~State();

	void addTransition(char input, vector<State*>* next_states);

	vector<State*>* getTransitions(char input);

	bool isAcceptor();

	int getAcceptorPriority();

	bool isNull();
};

#endif