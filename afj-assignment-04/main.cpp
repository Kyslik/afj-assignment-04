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

#include "lib/grammar.hpp"

const std::string INPUT_FILE = "./jflap-grammar.jff";
const std::string OUTPUT_FILE = "./out.txt";

void showHelp(char *s);
uint loadAutomata(afj_4::grammar::Grammar &grammar, const std::string &in);

int main (int argc, char *argv[])
{
    using std::cout;
    using std::endl;
    using std::string;

    if (argc == 1)
    {
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

    afj_4::types::Rules rules;
    afj_4::grammar::Grammar grammar(rules);

    if (input_file.empty()) input_file = INPUT_FILE;

    {
        int load = loadAutomata(grammar, input_file);

        if (load == 1)
        {
            cout << "File \"" << input_file << "\" does not exist." << endl;
            return 1;
        }

        if (load == 2)
        {
            cout << "File \"" << input_file << "\" is not \"grammar\" type. \nCreate new grammar in jFlap save it as *.jff and use that file." << endl;
            return 2;
        }
    }

    grammar.computeFirst();
    grammar.displayFirst();
    return 0;
}

void showHelp(char *s)
{
    using std::cout;
    using std::endl;

    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help" << endl;
    cout << "         " << "-i  [FILE] input file (xml format) / default \"" << INPUT_FILE << "\"" << endl;
    cout << "         " << "-o  [FILE] output file (xml format) / default \"" << OUTPUT_FILE << "\"" << endl;
    cout << "         " << "-w  [WORD] input word (epsilon character is SPACE)" << endl;
    cout << "         " << "WARNING: output file is always REWRITTEN" << endl;
    cout << endl;
    cout << endl;
}

uint loadAutomata(afj_4::grammar::Grammar &grammar, const std::string &in)
{
    using namespace pugi;
    using namespace afj_4;
    using std::string;

    xml_document doc;
    if (!doc.load_file(in.c_str())) return 1;

    xml_node structure = doc.child("structure");

    if (string(structure.child_value("type")) != "grammar") return 2;

    std::vector<string> lefts, rights;

    for (xml_node rule = structure.child("production"); rule; rule = rule.next_sibling("production"))
    {
        string left = string(rule.child_value("left")).c_str();
        string right = string(rule.child_value("right")).c_str();

        grammar.addNonterminal(left);
        grammar.addTerminal(right);

        lefts.push_back(left);
        rights.push_back(right);
    }

    for (uint i = 0; i < lefts.size(); i++)
    {
        std::string str = lefts.at(i);
        types::Rule rule(types::Nonterminal(str), lefts, rights.at(i));
        grammar.pushRule(rule);
    }

    return 0;
}