#include "Scanner.h"

Scanner::Scanner(Automata automata)
{
	this->automata = automata;
}



void Scanner::setInput(string input)
{
	input_idx = 0;	
	acceptor_encountered = false;
	last_acceptor = NULL;
	input_stream = input;
}

string Scanner::nextToken()
{
	automata.reset();
	acceptor_encountered = false;
	vector<char> chars;
	int local_acceptor_idx = -1;

	// Keep processing input until the eof or null state is reached
	while(input_idx < input_stream.size() && automata.getCurrentState().size() != 0)
	{
		char c = input_stream[input_idx];
		automata.consume(c);

		// Looks for the highest priority acceptor in current state and update last_acceptor, last_acceptor_idx
		for (auto state : automata.getCurrentState()){
			if (state->isAcceptor()){
				if (!acceptor_encountered || last_acceptor->getAcceptorPriority() < state->getAcceptorPriority())
					last_acceptor = state;
				acceptor_encountered = true;
				last_acceptor_idx = input_idx;
				local_acceptor_idx = chars.size();
			}
		}

		chars.push_back(c);
		input_idx++;
	}

	// Create a token out of chars
	auto token_end = acceptor_encountered ? chars.begin()+local_acceptor_idx+1 : chars.end();
	string token = string(chars.begin(), token_end);
	input_idx = acceptor_encountered ? last_acceptor_idx + 1 : input_idx;
	return token;
}

string Scanner::getLastTokenType()
{
	if (!acceptor_encountered) return "";
	return last_acceptor->getName();
}

bool Scanner::isFinished()
{
	return input_idx == input_stream.size();
}