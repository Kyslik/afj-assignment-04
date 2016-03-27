//
//  automaton.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 24/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include "automaton.hpp"


bool Automaton::accepts(string word)
{
    if (initial_state == -1 || dfa == false) return false;
    int current_state = initial_state;
    int transitions_traversed = 0;

    for (auto &ch : word)
    {
        string character(1, ch);
        if (alphabet.find(character) == alphabet.end()) return false;

        for (const auto &transition : transitions)
        {
            if (transition.from == current_state && transition.input == character)
            {
                current_state = transition.to;
                transitions_traversed++;
                break;
            }
        }
    }

    if (transitions_traversed == word.length() && states.at(current_state).final)
        return true;
    return false;
}

void Automaton::removeUnreachableStates()
{
    set<int> reachableStates;
    reachableStates.insert(initial_state);

    for (const auto &st : reachableStates) {
        for (auto &ch : alphabet)
        {
            if (ch.first == EPSILON_STRING) continue;
            vstate tmp = sortAndRemoveDuplicates(transitionsTo(st, ch.first));
            for (const auto &t : tmp)
                reachableStates.insert(t.id);
        }
    }

    if (reachableStates.size() == states.size()) return;

    vector<int> statesToRemove;
    for (const auto &st : states)
        if (reachableStates.find(st.first) == reachableStates.end()) statesToRemove.push_back(st.first);

    for (const auto &st : statesToRemove)
        removeState(st);

    vector<transition> transitionsToRemove;
    for (const auto &tr : transitions)
    {
        if (reachableStates.find(tr.from) == reachableStates.end() ||
            reachableStates.find(tr.to) == reachableStates.end())
            transitionsToRemove.push_back(tr);
    }
    for (const auto &tr : transitionsToRemove)
    {
        transitions.erase(remove(transitions.begin(), transitions.end(), tr), transitions.end());
    }
    //nfa2dfa();
}

void Automaton::nfa2dfa()
{
    vector<vstate> vstates;
    vector<transition> ntransitions;
    map<int, state> nstates;

    vstate init = eClosure(states[initial_state]);
    vstates.push_back(init);

    initial_state = 0;
    bool final = false;

    for (const auto &in : init)
        if (in.final) final = true;

    nstates[initial_state] = state(initial_state, groupStateName(init), true, final);
    int from = 0;
    for (int i = 0; i < vstates.size(); i++)
    {
        for (auto &ch : alphabet)
        {
            if (ch.first == EPSILON_STRING) continue;
            vstate tmp;
            bool final = false;

            for (const auto &s : vstates[i])
            {
                vstate t = transitionsTo(s, ch.first);
                tmp.insert(tmp.end(), t.begin(), t.end());
            }

            for (const auto &t : tmp)
                if (t.final) final = true;

            tmp = sortAndRemoveDuplicates(tmp);
            string sname = groupStateName(tmp);

            if (sname.empty()) continue;

            int it = vstateFind(vstates, sname);
            int to = 0;

            if (it == -1)
            {
                int id = (int) vstates.size();
                nstates[id] = state(id, sname, false, final);
                vstates.push_back(tmp);
                to = id;
            } else {
                to = it;
            }

            ntransitions.push_back(transition(from, to, ch.first, false));
        }
        from++;
    }

    states = nstates;
    transitions = ntransitions;
    determineType();
}

vstate Automaton::transitionsTo(state s, const string character)
{
    return transitionsTo(s.id, character);
}

vstate Automaton::transitionsTo(int id, const string character)
{
    vstate rstates;
    for (const auto &transition : transitions)
    {
        if (transition.from != id || transition.input != character) continue;

        rstates.push_back(states[transition.to]);
        vstate tmp = eClosure(states[transition.to]);
        rstates.insert(rstates.end(), tmp.begin(), tmp.end());
    }
    return rstates;
}

string Automaton::groupStateName(vstate vs)
{
    bool first = true;
    string name = "";

    for (const auto &s : vs)
    {
        if (!first) name += ", ";
        else first = false;
        name += s.name;
    }
    return name;
}

vstate Automaton::eClosure(state s)
{
    vstate rstates;
    rstates.push_back(s);

    for (const auto &t : transitions)
    {
        if (t.epsilon && s.id == t.from && !existsInVState(rstates, t.to))
        {
            vstate tmp = eClosure(states[t.to]);
            rstates.insert(rstates.end(), tmp.begin(), tmp.end());
        }
    }
    return sortAndRemoveDuplicates(rstates);
}

vstate Automaton::sortAndRemoveDuplicates(vstate rs)
{
    sort(rs.begin(), rs.end());
    rs.erase( unique( rs.begin(), rs.end() ), rs.end() );
    return rs;
}

int Automaton::vstateFind(vector<vstate> vs, string s)
{
    for (int i = 0; i < vs.size(); i++)
        if (s == groupStateName(vs[i])) return i;
    return -1;
}

bool Automaton::existsInVState(vstate vs, int id)
{
    for (const auto &s : vs)
    {
        if (s.id == id) return true;
    }
    return  false;
}

bool Automaton::existsInVState(vstate vs, state s)
{
    return existsInVState(vs, s.id);
}

void Automaton::determineType()
{
    dfa = true;

    map<int, int> state_transition;

    for (const auto &t : transitions)
    {
        if (t.input == EPSILON_STRING) //t.from != initial_state && 
        {
            dfa = false;
            return;
        }

        if (!state_transition.count(t.from))
            state_transition[t.from] = 0;
        state_transition.at(t.from)++;
    }

    size_t alphabet_size = alphabet.size();

    for (const auto &st : state_transition)
    {
        if (st.second > alphabet_size)
        {
            dfa = false;
            return;
        }
    }

    for (const auto &st : states)
    {
        const int id = st.second.id;
        map<string, bool> used_alpha;

        for (const auto &t : transitions)
        {
            if (t.from != id) continue;
            if (used_alpha.count(t.input))
            {
                dfa = false;
                return;
            }
            used_alpha[t.input] = true;
        }
    }
}