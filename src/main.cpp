#include "State.h"
#include "Automata.h"


int main(char argc[], int argv) {
	State state = State("state", true, 1, false);
	Automata automata = Automata(&state);
	return 0;
}