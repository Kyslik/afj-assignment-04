//
//  main.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 11/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include "lib/jFlapHandler.hpp"

int main(int argc, const char * argv[])
{

    xml_document doc;

    if (!doc.load_file("./jflap-automat.jff")) return -1;

    xml_node structure = doc.child("structure");
    xml_node automaton = structure.child("automaton");

    cout << structure.child_value("type") << endl;

    for (xml_node state = automaton.child("state"); state; state = state.next_sibling("state"))
    {
        cout << state.name() << endl;
    }

    for (xml_node transition = automaton.child("transition"); transition; transition = transition.next_sibling("transition"))
    {
        cout << transition.name() << endl;
    }

    return 0;
}
