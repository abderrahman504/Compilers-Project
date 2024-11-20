#include "State.h"
#include "Automata.h"


int main(char argc[], int argv) {
	State state = State("state", true, 1);
	Automata automata = Automata(&state);
	printf("Hello, World!");
	return 0;
}