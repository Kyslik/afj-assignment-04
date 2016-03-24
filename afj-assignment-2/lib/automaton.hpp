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
#include <vector>

#include "types.hpp"

using namespace std;

class Automaton {
public:
    map<int, state> states;
    vector<transition> transitions;

    inline void addState(state s)
    {
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
        transitions.push_back(t);
    }

    void load(string file);
};
#endif /* automaton_hpp */
