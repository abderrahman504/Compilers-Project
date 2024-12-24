#include "Parser.h"
#include "Grammar.h"
#include "iostream"


void printDerivation(vector<string> derivation){
	for(auto s : derivation) cout << s << " ";
	cout << endl;
}

Parser::Parser(string grammar_file) : grmr(grammar_file){
	start_symbol = grmr.getStartSymbol();
	eof = grmr.eof;
	epsilon = grmr.epsilon;
	parsingTable.constructTable(grmr);
	parsingTable.printTable();
}

void Parser::parse(vector<TableEntry> input)
{
	input.push_back(TableEntry(eof, eof, -1, -1));
	parseStack.push(eof);
	parseStack.push(start_symbol);

	vector<string> derivation = {start_symbol};
	int derivation_i = 0;
	int input_i = 0;
	printDerivation(derivation);
	while(parseStack.size() > 0 && input_i < input.size())
	{
		auto token = input[input_i];
		// If stack has non terminal
		if(grmr.getNonTerminals().count(parseStack.top()))
		{
			string reaction = parsingTable.getAction(parseStack.top(), token.type);
			if(reaction == parsingTable.synch){
				cout << "Sync err at ln " << token.line << ",col " << token.column << ": Expected " << parseStack.top() << "before " << token.token << endl;
				parseStack.pop();
				// Skipping a symbol in derivation because it can't be expanded
				derivation_i++;
			}
			else if(reaction == parsingTable.error){
				cout << "Error at ln" << token.line << ",col " << token.column << ": Unexpected " << token.token << endl;
				input_i++;
			}
			else
			{
				parseStack.pop();
				derivation.erase(derivation.begin()+derivation_i);
				istringstream iss(reaction);
				vector<string> production;
				string symbol;
				while (iss >> symbol) production.push_back(symbol);
				if(production[0] != epsilon){
					for(int i=production.size()-1; i>=0; i--){
						parseStack.push(production[i]);
						derivation.insert(derivation.begin()+derivation_i, production[i]);
					}
				}
				printDerivation(derivation);
			}
		}
		// If stack has terminal
		else{
			if(parseStack.top() != input[input_i].type){
				cout << "Error at ln" << token.line << ",col " << token.column << ": Unexpected " << token.token << endl;
				input_i++;
			}
			else{
				parseStack.pop();
				input_i++;
				derivation_i++;
			}
		}
	}
	if(input_i != input.size()){
		cout << "Parse stack empty but input still remaining.\n";
	}
	else if(parseStack.size() != 0){
		cout << "All input consumed but stack is not empty.\n";
	}
	else{
		cout << "Input parsed successfully.\n";
	}
}
