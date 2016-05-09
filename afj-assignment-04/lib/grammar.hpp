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
        using types::uint;

        typedef std::set<std::string> StringSet;
        
        class Grammar
        {
            types::Rules _rules;
            types::StringToTerminalSetMap   _firsts,
                                            _follows;

            StringSet   _terminals,
                        _nonterminals;

            bool insertInTerminalSet(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in);
            bool insertInFirst(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in);
            bool insertInFollow(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in);
            
            void displayTerminalSet(const types::StringToTerminalSetMap &terminal_set_map);
        public:
            Grammar();
            Grammar(const types::Rules &rules) : _rules(rules) {};

            inline void pushRule(const types::Rule &rule)
            {
                _rules.push_back(rule);
            }

            inline void addNonterminal(const std::string &nonterminal)
            {
                if (_terminals.find(nonterminal) != _terminals.end())
                    _terminals.erase(_terminals.find(nonterminal));
                _nonterminals.insert(nonterminal);
            }

            void addTerminal(const std::string &terminal);
            void computeFirst();
            void displayFirst();
            void displayFollow();
            void computeFollow();
        };
    }
}

#endif /* grammar_hpp */
