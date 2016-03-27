//
//  main.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 11/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <pugixml.hpp>
#include "lib/automaton.hpp"

using namespace pugi;
using namespace std;

void loadAutomata(Automaton &a, const string in);
void saveAutomata(Automaton &a, const string out);

int main(int argc, const char * argv[])
{

    Automaton automata;

    loadAutomata(automata, "jflap-automat-4.jff");
    automata.determineType();
    automata.nfa2dfa();
    automata.removeUnreachableStates();
    automata.minimise();
    saveAutomata(automata, "out.jff");
    return 0;
}

void loadAutomata(Automaton &a, const string in)
{
    xml_document doc;
    if (!doc.load_file(in.c_str())) return;

    xml_node structure = doc.child("structure");
    xml_node automaton = structure.child("automaton");

    if (string(structure.child_value("type")) != "fa") return;

    for (xml_node st = automaton.child("state"); st; st = st.next_sibling("state"))
    {
        bool initial = st.child("initial");
        bool final = st.child("final");
        int id = st.attribute("id").as_int();
        string name = st.attribute("name").as_string();

        a.addState(state(id, name, initial, final));
    }

    for (xml_node trans = automaton.child("transition"); trans; trans = trans.next_sibling("transition"))
    {
        int from = atoi(string(trans.child_value("from")).c_str());
        int to = atoi(string(trans.child_value("to")).c_str());
        string input = trans.child_value("read");
        bool epsilon = false;

        if (input.empty())
        {
            input = EPSILON_STRING;
            epsilon = true;
        }

        a.addTransition(transition(from, to, input, epsilon));
    }
}

void saveAutomata(Automaton &a, const string out)
{
    xml_document doc;
    auto declarationNode = doc.append_child(node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "UTF-8";
    declarationNode.append_attribute("standalone") = "no";
    auto root = doc.append_child("structure");

    xml_node nodeChild = root.append_child("type");
    nodeChild.append_child(pugi::node_pcdata).set_value("fa");

    xml_node n_automaton = root.append_child("automaton");
    for (const auto &st : a.states)
    {
        xml_node state = n_automaton.append_child("state");
        state.append_attribute("id") = st.second.id;
        state.append_attribute("name") = st.second.name.c_str();
        //        xml_node x = state.append_child("x");
        //        xml_node y = state.append_child("y");
        //        x.append_child(node_pcdata).set_value("0");
        //        y.append_child(node_pcdata).set_value("0");
        if (st.second.initial)
            state.append_child("initial");
        if (st.second.final)
            state.append_child("final");
    }

    for (const auto &tr : a.transitions)
    {
        xml_node transition = n_automaton.append_child("transition");
        xml_node from = transition.append_child("from");
        xml_node to = transition.append_child("to");
        xml_node read = transition.append_child("read");
        from.append_child(node_pcdata).set_value(to_string(tr.from).c_str());
        to.append_child(node_pcdata).set_value(to_string(tr.to).c_str());
        read.append_child(node_pcdata).set_value(tr.input.c_str());
    }
    
    doc.save_file(out.c_str());
}