//
//  main.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 11/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <pugixml.hpp>
#include "lib/automaton.hpp"


using namespace std;

void showHelp(char *s);
int loadAutomata(Automaton &a, const string in);
void saveAutomata(Automaton &a, const string out);

int main (int argc, char *argv[])
{

    if (argc == 1) {
        showHelp(argv[0]);
        return 0;
    }

    char get_opt;
    string input_file, output_file, word;

    while((get_opt = getopt(argc, argv, "hi:o:w:")) != -1)
    {
        switch(get_opt)
        {
            case '?':
                return 1;
            case 'h':
                showHelp(argv[0]);
                return 0;
            case 'i':
                input_file = strdup(optarg);
                break;
            case 'o':
                output_file = strdup(optarg);
                break;
            case 'w':
                word = strdup(optarg);
                break;
            default:
                showHelp(argv[0]);
                break;
        }
    }

    Automaton automata;

    if (input_file.empty()) input_file = INPUT_FILE;
    if (output_file.empty()) output_file = OUTPUT_FILE;

    int load = loadAutomata(automata, input_file);
    if (load == 1) {
        cout << "File \"" << input_file << "\" does not exist." << endl;
        return 1;
    }

    if (load == 2)
    {
        cout << "Automaton \"" << input_file << "\" is not \"fa\" type." << endl;
        return 2;
    }

    automata.determineType();
    cout << "Automata type: " << ((automata.dfa) ? "DFA" : "NFA") << endl;

    if (!automata.dfa)
    {
        cout << "Converting NFA to DFA:" << endl;
        cout << "\t NFA count of states: " << automata.states.size() << endl;
        cout << "\t NFA count of transitions: " << automata.transitions.size() << endl;
        automata.nfa2dfa();
        cout << "\t DFA count of states: " << automata.states.size() << endl;
        cout << "\t DFA count of transitions: " << automata.transitions.size() << endl;
        saveAutomata(automata, output_file);
        cout << endl;
        cout << "Automaton saved to: \"" << output_file << "\"" << endl;
        return 0;
    }

    if (word.empty())
    {
        cout << "Mnimizing DFA:" << endl;
        cout << "\t DFA count of states: " << automata.states.size() << endl;
        cout << "\t DFA count of transitions: " << automata.transitions.size() << endl;
        cout << "\t removing unreachable states..." << endl;
        automata.removeUnreachableStates();
        automata.minimize();
        cout << "\t minimizing..." << endl;
        cout << "\t Min DFA count of states: " << automata.states.size() << endl;
        cout << "\t Min DFA count of transitions: " <<automata.transitions.size() << endl;
        cout << endl;
        saveAutomata(automata, output_file);
        cout << "Minimized automaton saved to: \"" << output_file << "\"" << endl;
        return 0;
    }
    cout << "Checking word \"" << word << "\": " << endl;
    cout << (automata.accepts(word) ? "ACCEPTS" : "REJECTS") << endl;
    return 0;
}

void showHelp(char *s)
{
    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help" << endl;
    cout << "         " << "-i  [FILE] input file (xml format) / default \"" << INPUT_FILE << "\"" << endl;
    cout << "         " << "-o  [FILE] output file (xml format) / default \"" << OUTPUT_FILE << "\"" << endl;
    cout << "         " << "-w  [WORD] input word (epsilon character is SPACE)" << endl;
    cout << "         " << "WARNING: output file is always REWRITTEN" << endl;
    cout << endl;
    cout << "NFA - non deterministic finite automaton" << endl;
    cout << "DFA - deterministic finite automaton" << endl;
    cout << "Program checks if input (-i [default=\"" << INPUT_FILE << "\"]) automaton is NFA or DFA:" << endl;
    cout << "\t NFA - program converts automaton to DFA and saves it to output file (-o [default=\"" << OUTPUT_FILE << "\"])" << endl;
    cout << "\t DFA - program converts automaton to MINIMIZED DFA and saves it to output file (-o [default=\"" << OUTPUT_FILE << "\"])" << endl;
    cout << "\t DFA && -w option is set - program checks if input word is accepted or rejected by DFA automaton and writes result to console" << endl;
    cout << endl;
}

int loadAutomata(Automaton &a, const string in)
{
    using namespace pugi;
    xml_document doc;
    if (!doc.load_file(in.c_str())) return 1;

    xml_node structure = doc.child("structure");
    xml_node automaton = structure.child("automaton");

    if (string(structure.child_value("type")) != "fa") return 2;

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

    return 0;
}

void saveAutomata(Automaton &a, const string out)
{
    using namespace pugi;
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