#ifndef REWRITING_REVERSER_H
#define REWRITING_REVERSER_H
#include "rule.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace aspsio {

    //  The way the the Reversing Algorithm is performed
    //  0:  The body of a Rule with an auxiliar predicate in its head
    //      is, and with no auxiliar predicates in its body, is copied
    //      into all bodies which contain the auxiliar predicate.
    //  1:  All predicates of a Rule described above are substituted with
    //      the predicates contained by a Rule in which occurs the auxiliar
    //      predicate.
    
    enum ReversingMode {
        InsertionMode = 0,
        SubstitutionMode,
    };

    //  A generic class that does a Reverse of a rule that was rewritten
    //  during the grounding phase
    class RewritingReverser {

        protected:
            std::list<Rule*> *rules_to_reverse;
            std::unordered_map<int, Predicate*> *aux_predicates_instances;

        public:
            RewritingReverser(std::list<Rule*> &rules_set, std::unordered_map<int, Predicate*> &_aux_predicates_instances);
            virtual int RulesToOptimizeRemained();
            virtual void DoReverse();

    };
}

#endif