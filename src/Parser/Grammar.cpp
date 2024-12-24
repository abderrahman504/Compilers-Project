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
        while(first.count(epsilon) && cur < p.size()){
            first.erase(epsilon);
            auto additions = getFirst(p[cur]);
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
void Grammar::buildFollow(string symbol) {
    // If the follow set is already built, return.
    if (followSets.count(symbol)) return;
    
    // Initialize the follow set for the symbol.
    followSets[symbol] = FollowSet();
    if (symbol == getStartSymbol()) {
        followSets[symbol]["$"] = 0; // Assign a default priority for the end marker.
    }
    // if(symbol == getStartSymbol()) followSets[symbol][] = start_symbol;
    // Iterate through all productions to compute the Follow set for the symbol.
    //lhs = Non-terminal , rhs_list = all its productions in the grammar.
    for (const auto& [lhs, rhs_list] : productions) {
        //loop over all productions
        for (const auto& production : rhs_list) {
            //iterate through the production to catch the symbol we are computing its Follow set
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i] == symbol) {
                    size_t j = i + 1;

                    FollowSet follow;
                    bool nullable = true;

                    // Add First(beta) \ {ε} to Follow(symbol)
                    while (j < production.size() && nullable) {
                        nullable = false;

                        // Get the First set of the current symbol in the production.
                        auto first = getFirst(production[j]);
                        for (const auto& [elem, index] : first) {
                            if (elem != epsilon) {
                                follow[elem] = index; // Use the index for tracking.
                                
                            } else {
                                nullable = true;
                            }
                        }

                        break;
                    }

                    // If nullable or at the end, add Follow(lhs) to Follow(symbol).
                    if (nullable || j == production.size()) {
                        // Build the Follow set of LHS (if not already built).
                        buildFollow(lhs);

                        // Merge Follow(lhs) into Follow(symbol).
                        for (const auto& [elem, index] : followSets[lhs]) {
                            follow[elem] = index;
                        }
                    }

                    // Merge computed Follow into Follow(symbol).
                    for (const auto& [elem, index] : follow) {
                        followSets[symbol][elem] = index; // Update or insert into the map.
                    }
                }
            }
        }
    }
}

void Grammar::computeFollows() {
    // Ensure that Follow sets are computed for all non-terminals.
    for (const auto& non_terminal : non_terminals) {
        buildFollow(non_terminal);
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
            for (const auto& symbol : production) {
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
                for (const auto& [followSym,_] : followSets[nonTerminal]) {
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

const FollowSet Grammar::getFollow(string symbol){
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

const unordered_map<string, FollowSet> Grammar::getFollowSets() const {
    return followSets;
}

const unordered_set<string> Grammar::getTerminals() const {
    return terminals;
}

const unordered_set<string> Grammar::getNonTerminals() const {
    return non_terminals;
}