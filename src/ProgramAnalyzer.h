#ifndef PROGRAM_ANALYZER_H
#define PROGRAM_ANALYZER_H

#include "Scanner.h"



class ProgramAnalyzer {
	Scanner scanner;
	
	public:

	ProgramAnalyzer(Scanner scanner);


	void analyzeFile(std::string file_path);

};


#endif