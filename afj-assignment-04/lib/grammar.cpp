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
        displayTerminalSet(_firsts);
    }

    bool Grammar::insertInFirst(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in)
    {
        if(!insertInTerminalSet(terminal_set, terminal, in)) return false;

        std::cout << "inserting in FIRST(" << in << "): " << terminal.value << std::endl;
        return true;
    }

    void Grammar::computeFirst()
    {
        for (const auto &terminal : _terminals)
        {
            if (terminal == EPSILON) continue;
            _firsts[terminal] = types::TerminalSet({types::Terminal(terminal, false)});
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

    void Grammar::displayFollow()
    {
        displayTerminalSet(_follows);
    }

    bool Grammar::insertInFollow(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in)
    {
        if (!insertInTerminalSet(terminal_set, terminal, in)) return false;

        std::cout << "inserting in FOLLOW(" << in << "): " << terminal.value << std::endl;
        return true;
    }

    void Grammar::computeFollow()
    {
        insertInFollow(_follows[_rules[0].left.value], types::Terminal(EPSILON, true), _rules[0].left.value);
        while(true)
        {
            bool changed = false;
            for (const auto &rule : _rules)
            {
                auto &terminal_set = _follows[rule.left.value];

                for (uint i = 0; i < rule.right.unionals.size(); i++)
                {
                    auto unional = rule.right.unionals[i];
                    if (unional.type == TERMINAL) continue;

                    std::string nonterminal = unional.nonterminal.value;
                    size_t nonterminal_set_size = _follows[nonterminal].size();

                    if (i + 1 < rule.right.unionals.size())
                    {
                        std::string next_unional_value = rule.right.unionals[i + 1].getValue();

                        for (const auto &first : _firsts[next_unional_value])
                            if (!first.is_epsilon)
                                insertInFollow(_follows[nonterminal], first, nonterminal);

                        if (_firsts[next_unional_value].find(types::Terminal(EPSILON, true)) == _firsts[nonterminal].end())
                            for (const auto &terminal : terminal_set)
                                insertInFollow(_follows[nonterminal], terminal, nonterminal);
                    }
                    else
                        for (const auto &terminal : terminal_set)
                            insertInFollow(_follows[nonterminal], terminal, nonterminal);
                    if (!changed &&
                        (nonterminal_set_size != _follows[nonterminal].size())) changed = true;
                }
            }
            if (!changed) break;
        }
    }

    bool Grammar::insertInTerminalSet(types::TerminalSet &terminal_set, const types::Terminal &terminal, const std::string &in)
    {
        if (terminal_set.find(terminal) != terminal_set.end()) return false;
        terminal_set.insert(terminal);
        return true;
    }

    void Grammar::displayTerminalSet(const types::StringToTerminalSetMap &terminal_set_map)
    {
        for (const auto &set : terminal_set_map)
        {
            std::cout << set.first << ": ";
            for (const auto &item : set.second)
                std::cout << item.value << " ";
            std::cout << std::endl << std::endl;
        }
    }
}
}