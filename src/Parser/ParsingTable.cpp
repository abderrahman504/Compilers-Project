#include "ParsingTable.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>


std::string vectorToString(const std::vector<std::string>& vec, const std::string& delimiter = " ") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            oss << delimiter;
        }
        oss << vec[i];
    }
    return oss.str();
}

void ParsingTable::constructTable(const Grammar &grammar)
{
    // Step 1: Get the necessary data from the Grammar object
    for(auto symbol : grammar.getNonTerminals()) non_terminals.push_back(symbol);
    for(auto symbol : grammar.getTerminals()) terminals.push_back(symbol);
    
    auto productions = grammar.getProductions();   // Productions
    auto firstSets = grammar.getFirstSets();       // Precomputed FIRST sets
    auto followSets = grammar.getFollowSets();     // Precomputed FOLLOW sets
    auto epsilon = grammar.epsilon;                // Epsilon symbol

    // Step 2: Iterate over all non-terminals
    for (const auto &[nonTerminal, productionList] : productions)
    {
        const auto &firstSet = firstSets.at(nonTerminal);         // Get FIRST set for this non-terminal
        const auto &followSet = followSets.at(nonTerminal);       // Get FOLLOW set for this non-terminal
        
        // Iterate over all terminals
        for(auto terminal : terminals){
            string table_value;
            // Fill table entry with one of: -Production -Synch -Error
            if(firstSet.count(terminal)) table_value = vectorToString(productionList[firstSet.at(terminal)]);
            else if(followSet.count(terminal)){
                if(firstSet.count(epsilon)) table_value = vectorToString(productionList[firstSet.at(epsilon)]);
                else table_value = synch;
            }
            else table_value = error;
            table[nonTerminal][terminal] = table_value;
        }
    }
}

// Get action for a given non-terminal and terminal
string ParsingTable::getAction(const std::string &nonTerminal, const std::string &terminal) const
{
    auto itNonTerminal = table.find(nonTerminal);
    if (itNonTerminal != table.end())
    {
        auto itTerminal = itNonTerminal->second.find(terminal);
        if (itTerminal != itNonTerminal->second.end())
        {
            return itTerminal->second;
        }
    }
    return ""; // Return empty string if no action exists
}

// Print the parsing table
void ParsingTable::printTable() const
{
    FILE* out_file = fopen("Parsing_table.txt", "w");
    int column_widths[terminals.size()+1];
    // Find out the width of each column from the longest string in each of them
    // Terminals column
    int max = 0;
    for(auto non_term : non_terminals)
        max = non_term.size() > max ? non_term.size() : max;
    column_widths[0] = max;
    // Column of each terminal
    for(int i=0; i<terminals.size(); i++){
        max = terminals[i].size();
        for(auto &[_, row] : table){
            string cell = row.at(terminals[i]);
            max = cell.size() > max ? cell.size() : max;
        }
        column_widths[i+1] = max;
    }

    // Print header row of parse table
    // Print empty cell
    fprintf(out_file, "%*s", column_widths[0], "");
    // Print terminal cells
    for(int i=0; i<terminals.size(); i++){
        int excess = column_widths[i+1]-terminals[i].size();
        // I'm trying to center the word with empty space before and after it
        fprintf(out_file, "  %*s", column_widths[i+1]-excess/2, terminals[i].c_str());
        fprintf(out_file, "%*s", excess/2, "");
    }
    fprintf(out_file, "\n");
    
    // Print each row of table
    for (const auto &nonTerminal : non_terminals)
    {
        int excess = column_widths[0]-nonTerminal.size();
        // Printing non terminal 
        fprintf(out_file, "%*s", column_widths[0]-excess/2, nonTerminal.c_str());
        fprintf(out_file, "%*s", excess/2, "");
        for(int i=0; i<terminals.size(); i++){
            string cell_val = table.at(nonTerminal).at(terminals[i]);
            int excess = column_widths[i+1]-cell_val.size();
            // Printing table entry
            fprintf(out_file, "  %*s", column_widths[i+1]-excess/2, cell_val.c_str());
            fprintf(out_file, "%*s", excess/2, "");
        }
        fprintf(out_file, "\n");
    }
    fclose(out_file);
}
