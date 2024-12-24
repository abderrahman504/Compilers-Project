#include "Grammar.h"
#include "GrammerFileReader.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>


Grammar::Grammar(string grammer_file){
    GrammerFileReader reader;
    reader.readRules(grammer_file);
    start_symbol = reader.getStartSymbol();
    terminals = reader.getTerminals();
    terminals.insert(eof);
    non_terminals = reader.getNonTerminals();
    auto reader_productions = reader.getProductions();
    for(auto [symbol, prods] : reader_productions){
        for(auto p : prods) productions[symbol].push_back(Production(p));
    }
    computeFirsts();
    computeFollows();
}

void Grammar::buildFirst(string symbol){
    // If the first set is already built then return.
    if(firstSets.count(symbol)) return;
    firstSets[symbol] = FirstSet();

    vector<Production> prods = productions[symbol];
    for(int i=0; i<prods.size(); i++){
        Production& p = prods[i];
        int cur = 0;
        FirstSet first;
        first[epsilon] = i;
        // While the first set has epsi add the first of the next symbol in the production
        while(first.count(epsilon) && cur < p.symbols.size()){
            first.erase(epsilon);
            auto additions = getFirst(p.symbols[cur]);
            first.insert(additions.begin(), additions.end());
            cur++;
        }

        for(auto [elem, _] : first){
            first[elem] = i;
        }
        // For a LL(1) grammer there shouldn't be multiple productions generating the same element in FIRST
        // so I'm checking if somehow duplicates occur when merging the first from this productions with the existing FIRST set.
        int size_before = firstSets[symbol].size();
        // Merge first with set from firstSets
        firstSets[symbol].insert(first.begin(), first.end());
        if(firstSets[symbol].size() - size_before != first.size()){
            cout << "Duplicates while building FIRST(" << symbol <<"). Look into this plz.";
            cout << "This means that the same symbol in FIRST was generated from multiple productions\n";
        }
    }
}


void Grammar::computeFirsts() {
    for(auto symbol : non_terminals) buildFirst(symbol);
}

// Placeholder for Follow Set computation
void Grammar::computeFollows() {
    // Implementation of the Follow Set algorithm goes here
        bool changed = true;

    // Initialize Follow sets
    for (const auto& nonTerminal : non_terminals) {
        followSets[nonTerminal] = {};
    }

    // Add $ to the Follow set of the start symbol
    followSets[start_symbol].insert(eof);

    while (changed) {
        changed = false;

        for (const auto& [lhs, rhs_list] : productions) {
            for (const auto& production : rhs_list) {
                for (size_t i = 0; i < production.symbols.size(); ++i) {
                    const string& symbol = production.symbols[i];

                    if (std::find(non_terminals.begin(), non_terminals.end(), symbol) != non_terminals.end()) {
                        // Symbol is a non-terminal
                        bool nullable = true;

                        // Add First(beta) \ {λ} to Follow(symbol)
                        for (size_t j = i + 1; j < production.symbols.size(); ++j) {
                            const auto& beta = production.symbols[j];
                            if (beta.front() == '\'' && beta.back() == '\'') {
                                // Terminal: Add to Follow(symbol)
                                if (followSets[symbol].insert(beta).second) {
                                    changed = true;
                                }
                                nullable = false;
                                break;
                            } else {
                                // Non-terminal: Add First(beta) \ {λ}
                                for (const auto& [beta_first, _] : firstSets[beta]) {
                                    if (beta_first != epsilon && followSets[symbol].insert(beta_first).second) {
                                        changed = true;
                                    }
                                }

                                // If beta cannot derive λ, stop
                                if (firstSets[beta].find(epsilon) == firstSets[beta].end()) {
                                    nullable = false;
                                    break;
                                }
                            }
                        }

                        // If beta derives λ or is empty, add Follow(lhs) to Follow(symbol)
                        if (nullable || i + 1 == production.symbols.size()) {
                            for (const auto& lhs_follow : followSets[lhs]) {
                                if (followSets[symbol].insert(lhs_follow).second) {
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
        unordered_set<string> combinedFirst;

        for (const auto& production : rhs_list) {
            // Compute First(production)
            unordered_set<string> productionFirst;

            bool nullable = true;
            for (const auto& symbol : production.symbols) {
                if (symbol.front() == '\'' && symbol.back() == '\'') {
                    // Terminal: Add to First(production)
                    productionFirst.insert(symbol);
                    nullable = false;
                    break;
                } else if (symbol == epsilon) {
                    // Lambda: Continue to next symbol
                    productionFirst.insert(epsilon);
                } else {
                    // Non-terminal: Add First(symbol) \ {λ} to First(production)
                    for (const auto& [sym_first,_] : firstSets[symbol]) {
                        if (sym_first != epsilon) {
                            productionFirst.insert(sym_first);
                        }
                    }

                    // If symbol is not nullable, stop
                    if (firstSets[symbol].count(epsilon) == 0) {
                        nullable = false;
                        break;
                    }
                }
            }

            // If nullable, add λ to First(production)
            if (nullable) {
                productionFirst.insert(epsilon);
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
                for (const auto& followSym : followSets[nonTerminal]) {
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



const FirstSet Grammar::getFirst(string symbol){
    if(non_terminals.count(symbol)){
        if(firstSets.count(symbol) == 0) buildFirst(symbol);
        return firstSets[symbol]; 
    }
    else return FirstSet{{symbol, -1}};
}

const unordered_set<string> Grammar::getFollow(string symbol){
    return followSets[symbol];
}

string Grammar::getStartSymbol(){
    return start_symbol;
}

// Getter for productions
const unordered_map<string, vector<Production>> Grammar::getProductions() const {
    return productions;
}

const unordered_map<string, FirstSet> Grammar::getFirstSets() const {
    return firstSets;
}

const unordered_map<string, unordered_set<string>> Grammar::getFollowSets() const {
    return followSets;
}

const unordered_set<string> Grammar::getTerminals() const {
    return terminals;
}

const unordered_set<string> Grammar::getNonTerminals() const {
    return non_terminals;
}