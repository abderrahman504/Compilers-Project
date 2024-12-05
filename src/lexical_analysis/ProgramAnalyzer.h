#ifndef PROGRAM_ANALYZER_H
#define PROGRAM_ANALYZER_H

#include "Scanner.h"

struct TableEntry{
	string symbol;
	string type;

	TableEntry(std::string symbol, std::string type);
};

class ProgramAnalyzer {
	Scanner scanner;
	vector<TableEntry> symbols_table;

	public:

	ProgramAnalyzer(Scanner scanner);


	void analyzeFile(std::string file_path);

};


#endif