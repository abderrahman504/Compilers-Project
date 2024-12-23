#include "ParsingTable.h"
#include <iostream>
#include <iomanip>

void ParsingTable::constructTable(const Grammar &grammar)
{
    // Step 1: Get the necessary data from the Grammar object
    auto terminals = grammar.getTerminals();       // List of terminals
    auto nonTerminals = grammar.getNonTerminals(); // List of non-terminals
    auto productions = grammar.getProductions();   // Productions
    auto firstSets = grammar.getFirstSets();       // Precomputed FIRST sets
    auto followSets = grammar.getFollowSets();     // Precomputed FOLLOW sets
    auto epsilon = grammar.epsilon;                // Epsilon symbol

    // Step 2: Iterate over all non-terminals
    for (const auto &nonTerminal : nonTerminals)
    {
        const auto &productionList = productions.at(nonTerminal); // Get productions for this non-terminal
        const auto &firstSet = firstSets.at(nonTerminal);         // Get FIRST set for this non-terminal
        const auto &followSet = followSets.at(nonTerminal);       // Get FOLLOW set for this non-terminal

        for (const auto &firstElement : firstSet)
        {
            string element = firstElement.first;
            int productionIndex = firstElement.second;
            if (element == epsilon)
            {
                for (const auto &followElement : followSet)
                {
                    table[nonTerminal][followElement] = productionList[productionIndex].toString();
                }
            }
            else
            {
                table[nonTerminal][element] = productionList[productionIndex].toString();
            }
        }
    }
}

// Get action for a given non-terminal and terminal
std::string ParsingTable::getAction(const std::string &nonTerminal, const std::string &terminal) const
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
    for (const auto &nonTerminalEntry : table)
    {
        const auto &nonTerminal = nonTerminalEntry.first;
        for (const auto &terminalEntry : nonTerminalEntry.second)
        {
            const auto &terminal = terminalEntry.first;
            const auto &production = terminalEntry.second;
            std::cout << "(" << nonTerminal << ", " << terminal << ") -> " << production << std::endl;
        }
    }
}
