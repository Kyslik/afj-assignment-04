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
#include <math.h>


#include "types.hpp"
namespace afj_4
{
namespace automaton
{
    class Automaton
    {
        std::map<std::string, bool> alphabet;
        int initial_state = -1;

    public:
        std::map<int, types::state> states;
        std::vector<types::transition> transitions;
        bool dfa = true;

        inline void addState(types::state s)
        {
            if (s.initial) initial_state = s.id;
            states.insert(std::pair<int, types::state>(s.id, s));
        }

        inline void removeState(types::state s)
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

        inline bool existState(types::state s)
        {
            return existState(s.id);
        }

        inline types::state findState(int i)
        {
            return states.find(i)->second;
        }

        void addTransition(types::transition t)
        {
            alphabet[t.input] = true;
            transitions.push_back(t);
        }

        void    determineType();
        bool    accepts(std::string word);
        void    nfa2dfa();
        void    removeUnreachableStates();
        void    minimize();
        void    calculateXandY();

    private:
        types::vstate   eClosure(types::state s);
        types::vstate   transitionsTo(types::state s, const std::string character);
        types::vstate   transitionsTo(int id, const std::string character);
        int             transitionsToInt(int id, const std::string character);
        types::vstate   sortAndRemoveDuplicates(types::vstate rs);
        int             vstateFind(std::vector<types::vstate> v, std::string s);
        std::string     groupStateName(types::vstate vs);
        bool            existsInVState(types::vstate vs, int id);
        bool            existsInVState(types::vstate vs, types::state s);
        
    };
}
}
#endif /* automaton_hpp */
