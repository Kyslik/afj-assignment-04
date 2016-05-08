//
//  grammar.hpp
//  afj-assignment-04
//
//  Created by Martin Kiesel on 08/05/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef grammar_hpp
#define grammar_hpp

#include "types.hpp"

namespace afj_4
{
    namespace grammar
    {
        class Grammar
        {
            types::Rules rules;
        public:
            Grammar();
            Grammar(const types::Rules &rules) : rules(rules) {};
            void pushRule(const types::Rule &rule)
            {
                rules.push_back(rule);
            }
        };
    }
}

#endif /* grammar_hpp */
