//
//  types.hpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 24/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef types_h
#define types_h

#include <string>
#include <vector>
#include <map>

#define TERMINAL    0
#define NONTERMINAL 1

namespace afj_4
{

    const std::string EPSILON = "0";

namespace types
{
    typedef unsigned int uint;

    struct Terminal
    {
        bool    is_epsilon = true,
                is_empty = true;
        std::string value = "";

        Terminal() : value(EPSILON), is_epsilon(true), is_empty(false) {};
        Terminal(const std::string &value, bool epsilon = false) :
                value(value),
                is_epsilon(epsilon),
                is_empty (false) {}

        bool operator < (const Terminal& terminal) const
        {
            return (value < terminal.value);
        }

        bool operator > (const Terminal& terminal) const
        {
            return !(*this < terminal);
        }

        bool operator == (const Terminal& terminal) const
        {
            return (value == terminal.value && is_epsilon == terminal.is_epsilon && is_empty == terminal.is_empty);
        }

        bool operator != (const Terminal& terminal) const
        {
            return !(*this == terminal);
        }
    };

    typedef std::vector<Terminal> Terminals;
    typedef std::set<Terminal> TerminalSet;
    typedef std::map<std::string, TerminalSet> StringToTerminalSetMap;

    struct Nonterminal
    {
        bool is_empty = true;
        std::string value = "";

        Nonterminal() : value(""), is_empty(true) {};
        Nonterminal(const std::string &value) : value(value), is_empty(false) {}

        bool operator < (const Nonterminal& nonterminal) const
        {
            return (value < nonterminal.value);
        }

        bool operator > (const Nonterminal& nonterminal) const
        {
            return !(*this < nonterminal);
        }

        bool operator == (const Nonterminal& nonterminal) const
        {
            return (value == nonterminal.value && is_empty == nonterminal.is_empty);
        }

        bool operator != (const Nonterminal& nonterminal) const
        {
            return !(*this == nonterminal);
        }
    };

    typedef std::vector<Nonterminal> Nonterminals;

    struct Unional
    {
        Terminal    terminal;
        Nonterminal nonterminal;
        int type = -1;

        Unional()
        {
            terminal = Terminal();
            nonterminal = Nonterminal();
        };
        Unional(const Terminal &terminal) : terminal(terminal)
        {
            type = TERMINAL;
        };
        Unional(const Nonterminal &nonterminal) : nonterminal(nonterminal)
        {
            type = NONTERMINAL;
        };

        const std::string getValue() const
        {
            if(type == TERMINAL) return terminal.value;
            return nonterminal.value;
        }

        void setTerminal(const Terminal &_terminal)
        {
            terminal = _terminal;
            nonterminal = Nonterminal();
            type = TERMINAL;
        };
        void setNonterminal(const Nonterminal &_nonterminal)
        {
            terminal = Terminal();
            nonterminal = _nonterminal;
            type = NONTERMINAL;
        };
    private:
        //Unional();
        //Unional& operator=(const Unional&);
        //Unional(const Unional&);
    };

    typedef std::vector<Unional> Unionals;

    struct Right
    {
        std::string representation = "";
        Unionals unionals;

        void pushUnional(const Unional &unional)
        {
            if (unional.type == TERMINAL)
                pushTerminal(unional.terminal);
            else
                pushNonterminal(unional.nonterminal);
        };

        void pushTerminal(const Terminal &terminal)
        {
            representation += terminal.value;
            Unional unional(terminal);
            unionals.push_back(unional);
        };

        void pushNonterminal(const Nonterminal &nonterminal)
        {
            representation += nonterminal.value;
            Unional unional(nonterminal);
            unionals.push_back(unional);
        };
    };

    struct Rule
    {
        Nonterminal left;
        Right right;
        
        Rule(const Nonterminal &nonterminal) : left(nonterminal) {}
        Rule(const Nonterminal &nonterminal, const Right &right) :
                left(nonterminal),
                right(right) {}
        Rule(const Nonterminal &nonterminal, const std::vector<std::string> &lefts, const std::string &_right)
        {
            left = nonterminal;
            if (_right.size() == 0)
                pushUnional(Unional(Terminal(EPSILON, true))); // "" to EPSILON

            for(uint i = 0; i < _right.length(); i++)
            {
                std::string str("");
                str += _right.at(i);
                Unional u;

                if (std::find(lefts.begin(), lefts.end(), str) != lefts.end())
                    u.setNonterminal(Nonterminal(str));
                else
                    u.setTerminal(Terminal(str));

                pushUnional(Unional(u));
            }
        }

        void setLeft(const Nonterminal &nonterminal)
        {
            left = nonterminal;
        }

        void setRight(const Right &_right)
        {
            right = _right;
        }

        void pushUnional(const Unional &unional)
        {
            right.pushUnional(unional);
        }
    };

    typedef std::vector<Rule> Rules;

}
}
#endif /* types_h */