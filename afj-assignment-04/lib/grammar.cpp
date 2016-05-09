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

        if (terminal.empty())
            _terminals.insert(EPSILON);

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
                std::cout << item.value << " ";
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
            insertInFirst(terminal_set, rule.right.unionals[0].terminal, rule.left.value);
        }

        while (true)
        {
            bool changed = false;
            for (const auto &rule : _rules)
            {
                if (rule.right.unionals.size() == 0 ||
                    rule.right.unionals[0].type == TERMINAL) continue;

                std::cout << "rule: " << rule.left.value << " -> " << rule.right.representation << std::endl;

                auto &terminal_set = _firsts[rule.left.value];
                size_t terminal_set_size = terminal_set.size();

                {
                    auto unional = rule.right.unionals[0];
                    if (unional.type == TERMINAL && !unional.terminal.is_epsilon)
                        insertInFirst(terminal_set, unional.terminal, rule.left.value);

                    if (unional.type == NONTERMINAL)
                        for (const auto &first : _firsts[unional.nonterminal.value])
                            if (!first.is_epsilon)
                                insertInFirst(terminal_set, first, rule.left.value);
                }

                uint i = 0;
                while(i < rule.right.unionals.size() - 1 &&
                      rule.right.unionals[i].type == NONTERMINAL &&
                      _firsts[rule.right.unionals[i].nonterminal.value].find(types::Terminal(EPSILON, true)) != _firsts[rule.right.unionals[i].nonterminal.value].end())
                {
                    {
                        auto unional = rule.right.unionals[i+1];
                        if (unional.type == TERMINAL && !unional.terminal.is_epsilon)
                            insertInFirst(terminal_set, unional.terminal, rule.left.value);

                        if (unional.type == NONTERMINAL)
                            for (const auto &first : _firsts[unional.nonterminal.value])
                                if (!first.is_epsilon)
                                    insertInFirst(terminal_set, first, rule.left.value);
                    }
                    i++;
                }

                if(i == rule.right.unionals.size() &&
                   _firsts[rule.right.unionals[i].nonterminal.value].find(EPSILON) != _firsts[rule.right.unionals[i].nonterminal.value].end())
                    insertInFirst(terminal_set, types::Terminal(EPSILON, true), rule.left.value);

                if (!changed && terminal_set_size != terminal_set.size()) changed = true;
            }
            if (!changed) break;
        }
    }

    void Grammar::insertInFirst(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in)
    {
        if (terminal_set.find(terminal) != terminal_set.end()) return;
        terminal_set.insert(terminal);
        std::cout << "inserting in FIRST(" << in << "): " << terminal.value << std::endl;
    }
}
}