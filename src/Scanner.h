#ifndef SCANNER_H
#define SCANNER_H

#include "Automata.h"
#include "string"

using namespace std;

class Scanner
{
	Automata automata; // The automata used for scanning input.
	State* last_acceptor; // Stores the last acceptor state(s) encountered since the last call to nextToken().
	int last_acceptor_idx; // The index in the input stream where the last acceptor state was encountered.
	bool acceptor_encountered; // whether an acceptor state has been encountered since the last call to nextToken().
	int input_idx; // Tracks the progress of the automata through the input stream.
	string input_stream; // The input stream to read from.

	public:
	// Specify the Automata used for scanning input
	Scanner(Automata automata);

	void setInput(string input);

	// Returns the next valid token in the input stream, or the rest of the stream if no valid token was found
	string nextToken();
	
	// Use after `nextToken()` to get the type of the last valid token, or an empty string if the last token was invalid.
	string getLastTokenType();

	// Returns true if all input has been scanned, and false otherwise.
	bool isFinished();
	
};


#endif

