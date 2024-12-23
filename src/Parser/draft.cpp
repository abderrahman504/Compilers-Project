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

        // Step 3: For each production of the non-terminal
        for (const auto &production : productionList)
        {
            const auto &symbols = production.symbols;

            // Step 4: Get the FIRST set for this production directly
            std::unordered_set<std::string> productionFirstSet;
            bool isNullable = true;

            for (const auto &symbol : symbols)
            {
                if (terminals.count(symbol))
                {
                    // Terminal: Add it directly to the FIRST set
                    productionFirstSet.insert(symbol);
                    isNullable = false; // Terminal makes it non-nullable
                    break;
                }
                else if (nonTerminals.count(symbol))
                {
                    // Non-terminal: Add its FIRST set
                    const auto &symbolFirstSet = firstSets.at(symbol);
                    for (const auto &entry : symbolFirstSet)
                    {
                        const std::string &firstSymbol = entry.first; // Extract the string (key) from the inner map
                        productionFirstSet.insert(firstSymbol);       
                    }
                    if (symbolFirstSet.count(epsilon) == 0)
                    {
                        isNullable = false;
                        break;
                    }
                }
                else if (symbol == epsilon)
                {
                    // Production explicitly contains ε
                    productionFirstSet.insert(epsilon);
                    isNullable = true;
                    break;
                }
            }

            // Step 5: Fill the parsing table for all terminals in FIRST(production)
            for (const auto &term : productionFirstSet)
            {
                if (term != epsilon)
                {
                    table[nonTerminal][term] = production.toString();
                }
            }

            // Step 6: If nullable, fill the table using FOLLOW(nonTerminal)
            if (isNullable)
            {
                const auto &followSet = followSets.at(nonTerminal);
                for (const auto &term : followSet)
                {
                    table[nonTerminal][term] = production.toString();
                }
            }
        }
    }
}

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
