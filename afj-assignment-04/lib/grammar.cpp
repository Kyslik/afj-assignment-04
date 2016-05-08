//
//  grammar.cpp
//  afj-assignment-04
//
//  Created by Martin Kiesel on 08/05/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>

#include "grammar.hpp"

namespace afj_4
{
namespace grammar
{
    void Grammar::addTerminal(const std::string &terminal)
    {
        if (terminal.length() == 1 && _nonterminals.find(terminal) == _nonterminals.end())
            _terminals.insert(terminal);

        if (terminal.empty()) _terminals.insert(EPSILON);

        if (terminal.length() != 1)
        {
            for (uint i = 0; i < terminal.length(); i++)
            {
                std::string str("");
                str += terminal.at(i);

                addTerminal(str);
            }
        }
    }

    void Grammar::displayFirst()
    {
        for (const auto &first : _firsts)
        {
            std::cout << first.first << ": ";
            for (const auto &item : first.second)
                std::cout << item.value << ", ";
            std::cout << std::endl << std::endl;
        }
    }

    void Grammar::computeFirst()
    {
        for (const auto &terminal : _terminals)
        {
            bool epsilon = false;
            if (terminal == EPSILON) epsilon = true;
            _firsts[terminal] = types::TerminalSet({types::Terminal(terminal, epsilon)});
        }

        for (const auto &rule : _rules)
        {
            if (_firsts.find(rule.left.value) == _firsts.end())
                _firsts[rule.left.value] = types::TerminalSet();

            if (rule.right.unionals.size() == 0 ||
                rule.right.unionals[0].type == NONTERMINAL) continue;

            auto &terminal_set = _firsts[rule.left.value];

            terminal_set.insert(rule.right.unionals[0].terminal);
        }

        while (true)
        {
            bool changed = false;
            for (const auto &rule : _rules)
            {
                if (rule.right.unionals.size() == 0 ||
                    rule.right.unionals[0].type == TERMINAL) continue;

                auto &terminal_set = _firsts[rule.left.value];
                size_t terminal_set_size = terminal_set.size();

                bool has_predecessor_epsilon = false;
                bool terminated_by_terminal = false;

                for (const auto &unional : rule.right.unionals)
                {
                    if (unional.type == TERMINAL && !unional.terminal.is_epsilon)
                    {
                        terminal_set.insert(unional.terminal);
                        terminated_by_terminal = true;
                        break;
                    }

                    for (const auto &set : _firsts[unional.nonterminal.value])
                    {
                        if (!set.is_epsilon)
                        {
                            terminal_set.insert(set);
                        }
                        else
                            has_predecessor_epsilon = true;
                    }
                }

                if (has_predecessor_epsilon && !terminated_by_terminal)
                    terminal_set.insert(types::Terminal(EPSILON, true));

                if (terminal_set_size != terminal_set.size()) changed = true;
            }

            if (!changed) break;
        }
    }

//    void Grammar::computeFirst(const types::Rule &rule)
//    {
//        if (rule.right.unionals.size() == 0) return;
//
//        if (_firsts.find(rule.left.value) == _firsts.end())
//            _firsts[rule.left.value] = types::TerminalSet();
//
//        auto &terminal_set = _firsts[rule.left.value];
//
//        if (rule.right.unionals[0].type == TERMINAL)
//        {
//            terminal_set.insert(rule.right.unionals[0].terminal);
//            return;
//        }
//        else
//        {
//            for (const auto &unional : rule.right.unionals)
//            {
//                if (unional.type == TERMINAL) continue;
//                for (const auto &r : _rules)
//                {
//                    if (r.left == unional.nonterminal)
//                    {
//                        computeFirst(r);
//                    }
//                }
//            }
//
//            bool has_predecessor_epsilon = false;
//
//            for (const auto &unional : rule.right.unionals)
//            {
//                if (unional.type == TERMINAL && !unional.terminal.is_epsilon)
//                {
//                    terminal_set.insert(unional.terminal);
//                    return;
//                }
//
//                for (const auto &set : _firsts[unional.nonterminal.value])
//                {
//                    if (!set.is_epsilon)
//                        terminal_set.insert(set);
//                    else
//                        has_predecessor_epsilon = true;
//                }
//            }
//
//            if (has_predecessor_epsilon) terminal_set.insert(types::Terminal("", true));
//        }
//    };
//
//    void Grammar::computeFirst()
//    {
//        for (const auto &rule : _rules)
//        {
//            computeFirst(rule);
//        }
//    }
}
}