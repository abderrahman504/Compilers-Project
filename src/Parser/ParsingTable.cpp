#include "ParsingTable.h"
#include <iostream>
#include <iomanip>

// Hash function for pair<string, string> to use in unordered_map
namespace std
{
    template <>
    struct hash<std::pair<std::string, std::string>>
    {
        size_t operator()(const std::pair<std::string, std::string> &pair) const
        {
            return hash<std::string>()(pair.first) ^ hash<std::string>()(pair.second);
        }
    };
}

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
                    productionFirstSet.insert(symbolFirstSet.begin(), symbolFirstSet.end());
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
                    table[{nonTerminal, term}] = production.toString();
                }
            }

            // Step 6: If nullable, fill the table using FOLLOW(nonTerminal)
            if (isNullable)
            {
                const auto &followSet = followSets.at(nonTerminal);
                for (const auto &term : followSet)
                {
                    table[{nonTerminal, term}] = production.toString();
                }
            }
        }
    }
}

std::string ParsingTable::getAction(const std::string &nonTerminal, const std::string &terminal) const
{
    auto it = table.find({nonTerminal, terminal});
    if (it != table.end())
    {
        return it->second;
    }
    return ""; // Return empty string if no action exists
}

void ParsingTable::printTable() const
{
    for (const auto &entry : table)
    {
        const auto &[key, production] = entry;
        const auto &[nonTerminal, terminal] = key;
        std::cout << "(" << nonTerminal << ", " << terminal << ") -> " << production << std::endl;
    }
}
