//
//  types.hpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 24/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef types_h
#define types_h

#include <vector>
#include <string>
#include "constants.hpp"

struct position
{
    int x;
    int y;

    position()
    {
        x = 0;
        y = 0;
    }
    position(int _x, int _y) : x(_x), y(_y) {}
};

struct state
{
    int id;
    std::string name;
    bool initial;
    bool final;
    position jpos;
    state() {}
    state(int _id, std::string _name, bool _initial = false, bool _final = false, position _jpos = position()) :
        id(_id), name(_name), initial(_initial), final(_final), jpos(_jpos) {}

    bool operator < (const state& st) const
    {
        return (id < st.id);
    }

    bool operator == (const state& st) const
    {
        return (id == st.id);
    }

    bool operator != (const state& st) const
    {
        return !(*this == st);
    }


};

struct transition
{
    int from;
    int to;
    bool epsilon;
    std::string input;

    transition(int _from, int _to, std::string _input, bool _epsilon = false) : from(_from), to(_to), input(_input), epsilon(_epsilon) {}

    bool operator < (const transition& t) const
    {
        return (from < t.from);
    }
};

typedef std::vector<state> vstate;
#endif /* types_h */