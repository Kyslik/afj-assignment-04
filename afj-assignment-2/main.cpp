//
//  main.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 11/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <pugixml.hpp>
#include "constants.hpp"
#include "lib/automaton.hpp"

using namespace pugi;
using namespace std;

int main(int argc, const char * argv[])
{

    xml_document doc;
    Automaton automata;

    if (!doc.load_file("./jflap-automat.jff")) return -1;

    xml_node structure = doc.child("structure");
    xml_node automaton = structure.child("automaton");

    cout << structure.child_value("type") << endl;

    for (xml_node st = automaton.child("state"); st; st = st.next_sibling("state"))
    {
        bool initial = st.child("initial");
        bool final = st.child("final");
        int id = st.attribute("id").as_int();
        string name = st.attribute("name").as_string();

        automata.addState(state(id, name, initial, final));
    }

    for (xml_node trans = automaton.child("transition"); trans; trans = trans.next_sibling("transition"))
    {
        //automata.addTransition(transition(<#int _from#>, <#int _to#>, <#std::string _input#>));
    }

    return 0;
}