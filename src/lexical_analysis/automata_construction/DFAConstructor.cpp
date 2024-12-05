#include "DFAConstructor.h"
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include <iostream>

Automata DFAConstructor::constructDFA(const Automata &nfa)
{
    // Custom hash function for sets of NFA states
    struct SetHash
    {
        size_t operator()(const set<State *> &states) const
        {
            size_t hash = 0;
            for (auto state : states)
            {
                hash ^= std::hash<State *>()(state) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    // Custom equality function for sets of NFA states
    struct SetEqual
    {
        bool operator()(const set<State *> &lhs, const set<State *> &rhs) const
        {
            return lhs == rhs;
        }
    };

    unordered_map<set<State *>, State *, SetHash, SetEqual> dfaStates; // Maps sets of NFA states to DFA states
    queue<set<State *>> unprocessedStates;                             // Queue for BFS through DFA states

    // Get the epsilon-closure of the NFA's initial state
    State *nfaInitial = nfa.getInitialState();
    set<State *> startSet = epsilonClosure({nfaInitial});
    // Determine the name of the DFA's initial state
    string initialStateName;
    vector<State *> acceptorStates;
    for (State *state : startSet)
    {
        if (state->isAcceptor())
        {
            acceptorStates.push_back(state);
        }
    }

    if (!acceptorStates.empty())
    {
        // Sort acceptor states by priority to select the one with the highest priority
        auto highestPriorityState = *max_element(
            acceptorStates.begin(),
            acceptorStates.end(),
            [](State *a, State *b)
            {
                return a->getAcceptorPriority() < b->getAcceptorPriority();
            });

        initialStateName = highestPriorityState->getName();
    }
    else
    {
        // If no acceptor states, use the default "q0"
        initialStateName = "q0";
    }

    // Create the DFA's initial state
    State *dfaInitial = new State(initialStateName, containsAcceptor(startSet), highestPriority(startSet));
    dfaStates[startSet] = dfaInitial;

    unprocessedStates.push(startSet);
    Automata dfa(dfaInitial); // Initialize DFA with the initial state

    // Process each DFA state
    while (!unprocessedStates.empty())
    {
        set<State *> currentSet = unprocessedStates.front();
        unprocessedStates.pop();

        State *currentDFAState = dfaStates[currentSet];
        unordered_map<char, set<State *>> transitions;

        // Determine transitions for the current DFA state
        for (State *nfaState : currentSet)
        {
            for (auto &[input, nextStates] : nfaState->getTransitions())
            {
                if (input == 0)
                    continue; // Skip epsilon transitions
                for (State *nextState : nextStates)
                {
                    transitions[input].insert(nextState);
                }
            }
        }

        // Process transitions and create new DFA states as needed
        for (auto &[input, nfaTargetSet] : transitions)
        {
            set<State *> epsilonClosureSet = epsilonClosure(nfaTargetSet);
            if (dfaStates.find(epsilonClosureSet) == dfaStates.end())
            {
                // Create a new DFA state for this set of NFA states
                // Determine the name of the new DFA state based on your naming rules
                string stateName;

                vector<State *> acceptorStates;
                for (State *state : epsilonClosureSet)
                {
                    if (state->isAcceptor())
                    {
                        acceptorStates.push_back(state);
                    }
                }

                if (!acceptorStates.empty())
                {
                    // Sort acceptor states by priority to select the one with the highest priority
                    auto highestPriorityState = *max_element(
                        acceptorStates.begin(),
                        acceptorStates.end(),
                        [](State *a, State *b)
                        {
                            return a->getAcceptorPriority() < b->getAcceptorPriority();
                        });

                    stateName = highestPriorityState->getName();
                }
                else
                {
                    // If no acceptor states, use the default "q" naming
                    stateName = "q" + to_string(dfaStates.size());
                }
                State *newDFAState = new State(
                    stateName,
                    containsAcceptor(epsilonClosureSet),
                    highestPriority(epsilonClosureSet));
                dfaStates[epsilonClosureSet] = newDFAState;
                unprocessedStates.push(epsilonClosureSet);
            }
            currentDFAState->addTransition(input, vector<State *>{dfaStates[epsilonClosureSet]});
        }
    }

    return dfa;
}

// Computes the epsilon-closure of a set of states
set<State *> DFAConstructor::epsilonClosure(const set<State *> &states)
{
    set<State *> closure = states;
    queue<State *> toProcess;

    for (State *state : states)
    {
        toProcess.push(state);
    }

    while (!toProcess.empty())
    {
        State *state = toProcess.front();
        toProcess.pop();
        vector<State *> epsilonTransitions = state->getEpsilonTransitions();
        for (State *nextState : epsilonTransitions)
        {
            if (closure.find(nextState) == closure.end())
            {
                closure.insert(nextState);
                toProcess.push(nextState);
            }
        }
    }

    return closure;
}

// Checks if any state in the set is an acceptor state
bool DFAConstructor::containsAcceptor(const set<State *> &states)
{
    for (State *state : states)
    {
        if (state->isAcceptor())
            return true;
    }
    return false;
}

// Gets the highest priority among acceptor states in the set
int DFAConstructor::highestPriority(const set<State *> &states)
{
    int maxPriority = -1;
    for (State *state : states)
    {
        if (state->isAcceptor())
        {
            maxPriority = std::max(maxPriority, state->getAcceptorPriority());
        }
    }
    return maxPriority;
}

Automata DFAConstructor::minimizeDFA(const Automata &dfa)
{

    // Step 1: Separate states into partitions based on type
    unordered_map<string, unordered_set<State *>> partitionsMap;
    unordered_set<State *> allStates = dfa.getAllStates();

    for (State *state : allStates)
    {
        if (state->isAcceptor())
        {
            partitionsMap[state->getName()].insert(state);
        }
        else
        {
            partitionsMap[""].insert(state);
        }
    }


    // Convert map to vector of partitions for easier processing
    vector<unordered_set<State *>> partitions;
    for (const auto &[_, group] : partitionsMap)
    {
        partitions.push_back(group);
    }

    // Step 2: Iteratively refine partitions
    
    // Get the alphabet first
    unordered_set<char> alphabet;
    for (auto state : allStates){
        for (auto [input, _] : state->getTransitions()){
            alphabet.insert(input);
        }
    }
    
    // For each partition
    // hashset<State*> partition
    // if a partition is of size 1 then skip it.
    // hashmap<char, int> the partitions that a state goes to for a given input. -1 is the null partition
    // Compare the hashmaps of each state in a partition. Each group of equal maps is a new partition
    // For each state in partition
    
    bool updated = true;
    while (updated)
    {
        updated = false;
        vector<unordered_set<State *>> newPartitions;

        for (const auto partition : partitions)
        {
            if (partition.size() == 1){
                newPartitions.push_back(partition);
                continue;
            }
            vector<unordered_map<char, int>> transitionGroups;
            vector<State*> states;
            for (auto state : partition){
                states.push_back(state);
                unordered_map<char, int> map;
                for (char c : alphabet){
                    vector<State*> next = state->getTransitions(c);
                    if (next.size() == 0) map[c] = -1;
                    else{
                        // Figure out the index of the partition that next belongs to
                        for(int i=0; i<partitions.size(); i++){
                            if (partitions[i].find(next[0]) != partitions[i].end()){
                                map[c] = i;
                                break;
                            }
                            else if(i == partitions.size()-1){
                                cout << "Error: Couldn't find a state in any partition during minimization\n";
                            }
                        }
                    }
                }
                transitionGroups.push_back(map);
            }

            // Compare the maps. Identical ones belong to the same partition.

            unordered_set<int> ids_to_skip;
            for (int i=0; i<states.size(); i++){
                if (ids_to_skip.find(i) != ids_to_skip.end()) continue;
                unordered_set<State*> new_partition({states[i]});
                ids_to_skip.insert(i);
                for (int j = i+1; j<states.size(); j++){
                    if (ids_to_skip.find(j) != ids_to_skip.end()) continue;
                    if (transitionGroups[i] == transitionGroups[j]){
                        new_partition.insert(states[j]);
                        ids_to_skip.insert(j);
                    }
                }
                newPartitions.push_back(new_partition);
            }
        }
        if (newPartitions.size() > partitions.size()) updated = true;
        partitions = newPartitions;
    }

    // Step 3: Construct minimized DFA
    unordered_map<State *, State *> stateMapping;
    vector<State *> minimizedStates;

    for (const auto &partition : partitions)
    {
        // Determine the name of the new minimized state based on your naming rules
        string stateName;

        vector<State *> acceptorStates;
        for (State *state : partition)
        {
            if (state->isAcceptor())
            {
                acceptorStates.push_back(state);
            }
        }

        if (!acceptorStates.empty())
        {
            // Sort acceptor states by priority to select the one with the highest priority
            auto highestPriorityState = *max_element(
                acceptorStates.begin(),
                acceptorStates.end(),
                [](State *a, State *b)
                {
                    return a->getAcceptorPriority() < b->getAcceptorPriority();
                });
            stateName = highestPriorityState->getName();
        }
        else
        {
            // If no acceptor states, use the default "q" naming
            stateName = "q" + to_string(minimizedStates.size());
        }

        // Create a representative state for this partition
        State *representative = *partition.begin();
        State *newState = new State(stateName, representative->isAcceptor(), representative->getAcceptorPriority());
        minimizedStates.push_back(newState);

        // Map old states in the partition to the new state
        for (State *oldState : partition)
        {
            stateMapping[oldState] = newState;
        }
    }

    // Add transitions to minimized states
    for (State *state : minimizedStates)
    {
        auto it = find_if(stateMapping.begin(), stateMapping.end(),
                          [state](const auto &pair)
                          { return pair.second == state; });
        State *originalState = it->first;

        for (auto &[input, nextStates] : originalState->getTransitions())
        {
            if (!nextStates.empty())
            {
                state->addTransition(input, {stateMapping[nextStates[0]]});
            }
        }
    }

    // Return the minimized DFA
    State *minimizedInitial = stateMapping[dfa.getInitialState()];
    Automata minimizedDFA(minimizedInitial);
    return minimizedDFA;
}


#include <iomanip>   // For setw
#include <fstream>   // For ofstream

void DFAConstructor::printTransitionTable(const Automata &dfa) {
    // Open file stream
    std::string fileName = "../Transition_table.txt";
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << " for writing." << std::endl;
        return;
    }

    // Get all states and initialize structures
    unordered_set<State *> allStates = dfa.getAllStates();
    set<char> allInputs; // To store all possible input characters

    // Determine all possible input characters
    for (State *state : allStates) {
        for (auto &transition : state->getTransitions()) {
            allInputs.insert(transition.first);
        }
    }

    // Calculate column widths
    size_t stateColumnWidth = 15;
    size_t inputColumnWidth = 10;

    // Write the table header
    outFile << std::left << std::setw(stateColumnWidth) << "State";
    for (char input : allInputs) {
        outFile << std::setw(inputColumnWidth) << input;
    }
    outFile << std::endl;

    outFile << std::string(stateColumnWidth + inputColumnWidth * allInputs.size(), '-') << std::endl;

    // Write each state and its transitions
    for (State *state : allStates) {
        outFile << std::left << std::setw(stateColumnWidth) << state->getName(); // State name in the first column

        // Get all transitions as a map
        auto transitions = state->getTransitions();

        // Loop through all inputs in allInputs
        for (char input : allInputs) {
            // Check if the input exists in the transitions map directly
            if (transitions.count(input) > 0) {
                // Retrieve the vector of target states for the input
                vector<State *> targetStates = transitions[input];
                
                // If the vector is not empty, write the first target state
                if (!targetStates.empty()) {
                    outFile << std::setw(inputColumnWidth) << targetStates[0]->getName();
                } else {
                    // No transitions for this input
                    outFile << std::setw(inputColumnWidth) << " ";
                }
            } else {
                // No transition for this input
                outFile << std::setw(inputColumnWidth) << "-";
            }
        }


        outFile << std::endl;
    }

    // Close file stream
    outFile.close();
    std::cout << "Transition table successfully written to " << fileName << std::endl;
}

