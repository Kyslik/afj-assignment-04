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

namespace afj_4
{
namespace types
{
    struct position
    {
        int x,
            y;

        position()
        {
            x = 0;
            y = 0;
        }
        position(int x, int y) : x(x), y(y) {}
        position& operator =(const position& a)
        {
            x = a.x;
            y = a.y;
            return *this;
        }
    };

    struct state
    {
        int id;
        bool initial,
             final;

        std::string name;
        position jpos;

        state() {}
        state(int id, std::string name, bool initial = false, bool final = false, position jpos = position()) :
        id(id), name(name), initial(initial), final(final), jpos(jpos) {}

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
        int from,
        to;
        bool epsilon;

        std::string input;

        transition(int from, int to, std::string input, bool epsilon = false) : from(from), to(to), input(input), epsilon(epsilon) {}

        bool operator < (const transition& t) const
        {
            return (from < t.from);
        }
        
        bool operator == (const transition& t) const
        {
            return (from == t.from && to == t.to && epsilon == t.epsilon && input == t.input);
        }
        
        bool operator != (const transition& t) const
        {
            return !(*this == t);
        }
    };
    typedef std::vector<state> vstate;
}
}
#endif /* types_h */