#include "NFABuilder.h"
#include <vector>

NFABuilder::NFABuilder(const FileParser& parser) : stateCounter(0), fullNFA(createState()) {
    std::vector<Automata> nfas;

    // Convert regular expressions to NFAs
    const auto& regularExpressions = parser.getRegularExpressions();
    for (const auto& [name, regex] : regularExpressions) {
        Automata nfa = regexToNFA(regex);
        nfas.push_back(nfa);
    }

    // Convert keywords into NFAs
    const auto& keywords = parser.getKeywords();
    for (const auto& keyword : keywords) {
        Automata keywordNFA = regexToNFA(keyword);
        nfas.push_back(keywordNFA);
    }

    // Convert punctuations into NFAs
    const auto& punctuations = parser.getPunctuations();
    for (const auto& punctuation : punctuations) {
        Automata punctuationNFA = regexToNFA(punctuation);
        nfas.push_back(punctuationNFA);
    }

    // Combine all NFAs into a single NFA
    fullNFA = combineNFAs(nfas);
}
