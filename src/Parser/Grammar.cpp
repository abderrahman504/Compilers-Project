#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <regex>
#include <algorithm>

// Utility function to trim whitespace
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Load CFG from file and populate Grammar data structures
void Grammar::loadFromFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line, lhs, current_rhs;
    bool is_rhs_continuing = false;

    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Skip empty or comment lines

        if (!is_rhs_continuing) {
            auto pos = line.find("::=");
            if (pos == std::string::npos) {
                throw std::runtime_error("Invalid production rule format: " + line);
            }

            lhs = trim(line.substr(0, pos));
            current_rhs = line.substr(pos + 3);

            // Add LHS to nonTerminals if not already present
            if (std::find(nonTerminals.begin(), nonTerminals.end(), lhs) == nonTerminals.end()) {
                nonTerminals.push_back(lhs);
            }
        } else {
            current_rhs += " " + line;
        }

        if (line.back() == '\\') {
            is_rhs_continuing = true;
            current_rhs.pop_back(); // Remove the backslash
        } else {
            is_rhs_continuing = false;

            // Parse the RHS into individual productions
            std::vector<std::vector<std::string>> rhs_productions;
            std::stringstream ss(current_rhs);
            std::string option;
            while (std::getline(ss, option, '|')) {
                std::vector<std::string> tokens;
                std::stringstream option_stream(trim(option));
                std::string token;

                while (option_stream >> token) {
                    // Check if the token is a terminal
                    if (token.front() == '\'' && token.back() == '\'') {
                        terminals.push_back(token);
                    }

                    tokens.push_back(token);
                }
                rhs_productions.push_back(tokens);
            }

            // Add to productions map
            productions[lhs].insert(productions[lhs].end(), rhs_productions.begin(), rhs_productions.end());
        }
    }

    // Remove duplicate terminals
    std::sort(terminals.begin(), terminals.end());
    terminals.erase(std::unique(terminals.begin(), terminals.end()), terminals.end());

    infile.close();
}

// Placeholder for First Set computation
void Grammar::computeFirstSet() {
    // Implementation of the First Set algorithm goes here
    bool changed = true;

    // Initialize First sets
    for (const auto& nonTerminal : nonTerminals) {
        firstSet[nonTerminal] = {};
    }

    while (changed) {
        changed = false;

        for (const auto& [lhs, rhs_list] : productions) {
            for (const auto& production : rhs_list) {
                bool nullable = true;

                for (const auto& symbol : production) {
                    if (symbol.front() == '\'' && symbol.back() == '\'') {
                        // Terminal: Add to First(lhs)
                        if (firstSet[lhs].insert(symbol).second) {
                            changed = true;
                        }
                        nullable = false;
                        break;
                    } else if (symbol == "\\L") {
                        // Lambda: Add to First(lhs)
                        if (firstSet[lhs].insert("\\L").second) {
                            changed = true;
                        }
                    } else {
                        // Non-terminal: Add First(symbol) \ {λ} to First(lhs)
                        for (const auto& sym_first : firstSet[symbol]) {
                            if (sym_first != "\\L" && firstSet[lhs].insert(sym_first).second) {
                                changed = true;
                            }
                        }

                        // If symbol cannot derive λ, stop
                        if (firstSet[symbol].find("\\L") == firstSet[symbol].end()) {
                            nullable = false;
                            break;
                        }
                    }
                }

                // If all symbols in production can derive λ, add λ to First(lhs)
                if (nullable && firstSet[lhs].insert("\\L").second) {
                    changed = true;
                }
            }
        }
    }
}

// Placeholder for Follow Set computation
void Grammar::computeFollowSet() {
    // Implementation of the Follow Set algorithm goes here
        bool changed = true;

    // Initialize Follow sets
    for (const auto& nonTerminal : nonTerminals) {
        followSet[nonTerminal] = {};
    }

    // Add $ to the Follow set of the start symbol
    followSet[nonTerminals[0]].insert("$");

    while (changed) {
        changed = false;

        for (const auto& [lhs, rhs_list] : productions) {
            for (const auto& production : rhs_list) {
                for (size_t i = 0; i < production.size(); ++i) {
                    const std::string& symbol = production[i];

                    if (std::find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end()) {
                        // Symbol is a non-terminal
                        bool nullable = true;

                        // Add First(beta) \ {λ} to Follow(symbol)
                        for (size_t j = i + 1; j < production.size(); ++j) {
                            const auto& beta = production[j];
                            if (beta.front() == '\'' && beta.back() == '\'') {
                                // Terminal: Add to Follow(symbol)
                                if (followSet[symbol].insert(beta).second) {
                                    changed = true;
                                }
                                nullable = false;
                                break;
                            } else {
                                // Non-terminal: Add First(beta) \ {λ}
                                for (const auto& beta_first : firstSet[beta]) {
                                    if (beta_first != "\\L" && followSet[symbol].insert(beta_first).second) {
                                        changed = true;
                                    }
                                }

                                // If beta cannot derive λ, stop
                                if (firstSet[beta].find("\\L") == firstSet[beta].end()) {
                                    nullable = false;
                                    break;
                                }
                            }
                        }

                        // If beta derives λ or is empty, add Follow(lhs) to Follow(symbol)
                        if (nullable || i + 1 == production.size()) {
                            for (const auto& lhs_follow : followSet[lhs]) {
                                if (followSet[symbol].insert(lhs_follow).second) {
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Check if the grammar is LL(1)
bool Grammar::isLL1() {
    // Implementation for LL(1) grammar check goes here
    for (const auto& [nonTerminal, rhs_list] : productions) {
        std::unordered_set<std::string> combinedFirst;

        for (const auto& production : rhs_list) {
            // Compute First(production)
            std::unordered_set<std::string> productionFirst;

            bool nullable = true;
            for (const auto& symbol : production) {
                if (symbol.front() == '\'' && symbol.back() == '\'') {
                    // Terminal: Add to First(production)
                    productionFirst.insert(symbol);
                    nullable = false;
                    break;
                } else if (symbol == "\\L") {
                    // Lambda: Continue to next symbol
                    productionFirst.insert("\\L");
                } else {
                    // Non-terminal: Add First(symbol) \ {λ} to First(production)
                    for (const auto& sym_first : firstSet[symbol]) {
                        if (sym_first != "\\L") {
                            productionFirst.insert(sym_first);
                        }
                    }

                    // If symbol is not nullable, stop
                    if (firstSet[symbol].find("\\L") == firstSet[symbol].end()) {
                        nullable = false;
                        break;
                    }
                }
            }

            // If nullable, add λ to First(production)
            if (nullable) {
                productionFirst.insert("\\L");
            }

            // Check for overlap in combinedFirst and productionFirst
            for (const auto& sym : productionFirst) {
                if (combinedFirst.find(sym) != combinedFirst.end()) {
                    // Conflict detected in FIRST sets
                    return false;
                }
                combinedFirst.insert(sym);
            }

            // If production is nullable, check FOLLOW set
            if (nullable) {
                for (const auto& followSym : followSet[nonTerminal]) {
                    if (combinedFirst.find(followSym) != combinedFirst.end()) {
                        // Conflict between FIRST and FOLLOW sets
                        return false;
                    }
                }
            }
        }
    }

    return true;
}



// Getter for productions
const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& Grammar::getProductions() const {
    return productions;
}
