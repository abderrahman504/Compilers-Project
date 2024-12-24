#include "Parser.h"
#include "Grammar.h"


Parser::Parser(string grammar_file){
	Grammar grmr(grammar_file);
	start_symbol = grmr.getStartSymbol();
	eof = grmr.eof;
	epsilon = grmr.epsilon;
	parsingTable.constructTable(grmr);
	parsingTable.printTable();

}

void Parser::parse(vector<TableEntry> input){
	
}
