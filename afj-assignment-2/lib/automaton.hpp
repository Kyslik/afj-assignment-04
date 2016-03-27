//
//  automaton.hpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 24/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef automaton_hpp
#define automaton_hpp

#include <map>
#include <set>
#include <vector>
#include <string>

#include "types.hpp"

using namespace std;

class Automaton
{
    map<string, bool> alphabet;
    int initial_state = -1;

public:
    map<int, state> states;
    vector<transition> transitions;
    bool dfa = true;
    
    inline void addState(state s)
    {
        if (s.initial) initial_state = s.id;
        states.insert(pair<int, state>(s.id, s));
    }

    inline void removeState(state s)
    {
        removeState(s.id);
    }

    inline void removeState(int i)
    {
        states.erase(i);
    }

    inline bool existState(int i)
    {
        if (states.find(i) == states.end()) return false;
        return true;
    }

    inline bool existState(state s)
    {
        return existState(s.id);
    }
    
    inline state findState(int i)
    {
        return states.find(i)->second;
    }

    void addTransition(transition t)
    {
        alphabet[t.input] = true;
        transitions.push_back(t);
    }

    void determineType();
    bool accepts(string word);
    void nfa2dfa();
    void removeUnreachableStates();

private:
    vstate eClosure(state s);
    vstate transitionsTo(state s, string character);
    vstate transitionsTo(int id, string character);
    vstate sortAndRemoveDuplicates(vstate rs);
    int vstateFind(vector<vstate> v, string s);
    string groupStateName(vstate vs);
    bool existsInVState(vstate vs, int id);
    bool existsInVState(vstate vs, state s);

};

#endif /* automaton_hpp */
