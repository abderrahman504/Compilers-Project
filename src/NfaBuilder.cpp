#include "NFABuilder.h"
#include "Automata.h"
#include <stack>
#include <unordered_map>

#include "NFABuilder.h"
#include <vector>
#include <iostream>

NFABuilder::NFABuilder() : stateCounter(0) {}


State* NFABuilder::createState(bool isAcceptor, int acceptorPriority) {
    std::string stateName = "q" + std::to_string(stateCounter++);
    return new State(stateName, isAcceptor, acceptorPriority);
}


/**
 * @brief Builds a Non-Deterministic Finite Automaton (NFA) from a regular expression using Thomson's Construction.
 * 
 * @param regex The regular expression to convert into an NFA.
 * @return Automata The constructed NFA representing the given regular expression.
 */
void NFABuilder::regexToNFA(const std::string& regex, const std::string& name, int priority, State** out_initial, State** out_acceptor) {
    std::stack<vector<State*>> strays;
    std::stack<State*> initials;
    State* current = createState(false);
    initials.push(current);
    strays.push(vector<State*>());
    for (int i=0; i<regex.size(); i++)
    {
        char c = regex[i];
        // Ignore blanks
        if (c == ' ') continue;
        // Handle all special characters
        if (c == '('){
            initials.push(createState(false));
            current->addEpsilonTransition(vector<State*>{initials.top()});
            strays.push(vector<State*>());
            current = initials.top();
        }
        else if (c == ')'){
            State* new_state = createState(false);
            for (auto stray : strays.top()){
                stray->addEpsilonTransition(vector<State*>{new_state});
            }
            strays.pop();
            initials.pop();
            current = new_state;
        }
        else if (c == '+'){
            current->addEpsilonTransition(vector<State*>{initials.top()});
        }
        else if (c == '*'){
            current->addEpsilonTransition(vector<State*>{initials.top()});
            initials.top()->addEpsilonTransition(vector<State*>{current});
        }
        else if (c == '|'){
            strays.top().push_back(current);
            current = initials.top();
        }
        // Handle escaped characters
        else if (c == '\\'){ 
            i++;
            c = regex[i];
            // If it's the lambda character...
            if (c == 'L'){
                State* new_state = createState(false);
                initials.top()->addEpsilonTransition(vector<State*>{current});
            }
            // Anything else...
            else{
                State* new_state = createState(false);
                current->addTransition(c, vector<State*>{new_state});
                current = new_state;
            }
        }
        // Handle normal input
        else{
            State* new_state = createState(false);
            current->addTransition(c, vector<State*>{new_state});
            // If the next character is - then this is a range
            if(i < regex.size()-1 && regex[i+1] == '-'){
                i += 2;
                char end = regex[i];
                for(char x = c+1; x <= end; x++){
                    current->addTransition(x, vector<State*>{new_state});
                }
            }
            current = new_state;
        }
    }
    // At this point both initials and strays stacks should be of size 1
    if (initials.size() != 1){
        std::cerr << "Error: initials stack was of size " << initials.size() << " after making " << name << std::endl;
    }
    if (strays.size() != 1){
        std::cerr << "Error: strays stack was of size " << strays.size() << " after making " << name << std::endl;
    }

    // Add the final state and connect all strays to it
    State* final = new State(name, true, priority);
    for (auto stray : strays.top()){
        stray->addEpsilonTransition(vector<State*>{final});
    }
    out_initial = &initials.top();
    out_acceptor = &final;
}


Automata NFABuilder::getFullNFA(unordered_map<string, string> regularExpressions, vector<string> keywords, vector<string> punctuations)
{
    std::vector<State*> all_initials;
    for (auto [name, regex] : regularExpressions) {
        std::cout <<name <<" : " << regex;
        State* initial;
        regexToNFA(regex, name, 0, &initial, nullptr);
        all_initials.push_back(initial);
    }
    std::cout << "Converted Regular Expressions to NFA";

    // Convert keywords into NFAs
    for (auto keyword : keywords) {
        State* initial;
        regexToNFA(keyword, keyword, 1, &initial, nullptr);
        all_initials.push_back(initial);
    }
    std::cout << "Converted Keywords to NFA";

    // Convert punctuations into NFAs
    for (auto punctuation : punctuations) {
        State* initial;
        regexToNFA(punctuation, punctuation, 1, &initial, nullptr);
        all_initials.push_back(initial);
    }
    std::cout << "Converted Punctuations to NFA";

    // Combine all NFAs into a single NFA
    State* startState = new State("Start", false, -1);
    for (auto initial : all_initials) {
        startState->addEpsilonTransition(std::vector<State*>({initial}));
    }

    Automata nfa = Automata(startState);
    return nfa;
}
