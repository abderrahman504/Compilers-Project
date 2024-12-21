#ifndef PROGRAM_ANALYZER_H
#define PROGRAM_ANALYZER_H

#include "Scanner.h"

struct TableEntry{
	const string token;
	const string type;
	const int line;
	const int column;

	TableEntry(std::string token, std::string type, int line, int column);
};

class ProgramAnalyzer {
	Scanner scanner;
	vector<TableEntry> symbols_table;

	public:

	ProgramAnalyzer(Scanner scanner);

	// Parses the given program and returns the successfully matched tokens with metadata.
	vector<TableEntry> analyzeFile(std::string file_path);


};


#endif