//
//  grammar.hpp
//  afj-assignment-04
//
//  Created by Martin Kiesel on 08/05/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef grammar_hpp
#define grammar_hpp

#include <string>
#include <set>

#include "types.hpp"

namespace afj_4
{
    namespace grammar
    {
        typedef std::set<std::string> StringSet;
        using types::uint;
        class Grammar
        {
            types::Rules rules;
            StringSet terminals, nonterminals;

        public:
            Grammar();
            Grammar(const types::Rules &rules) : rules(rules) {};
            void pushRule(const types::Rule &rule)
            {
                rules.push_back(rule);
            }

            void addTerminal(const std::string &terminal)
            {
                if (terminal.length() == 1 && nonterminals.find(terminal) == nonterminals.end())
                    terminals.insert(terminal);
                if (terminal.length() != 1)
                {
                    for (uint i = 0; i < terminal.length(); i++)
                    {
                        std::string str("");
                        str += terminal.at(i);

                        addTerminal(str);
                    }
                }
                if (terminal.empty()) terminals.insert(EPSILON);
            };

            void addNonterminal(const std::string &nonterminal)
            {
                if (terminals.find(nonterminal) != terminals.end())
                {
                    terminals.erase(terminals.find(nonterminal));
                }
                nonterminals.insert(nonterminal);
            };
        };
    }
}

#endif /* grammar_hpp */
