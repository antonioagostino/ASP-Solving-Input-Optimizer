#ifndef ASPIF_REWRITING_REVERSER_H
#define ASPIF_REWRITING_REVERSER_H
#include "aspif_statement.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace aspsio {

    //  A specific "aspif" format class that does a Reverse 
    //  of a rule that was rewritten during the grounding phase
    class AspifRewritingReverser {

        protected:
            std::list<AspifStatement*> *rules_to_reverse;
            std::unordered_map<int, AspifLiteral*> *aux_predicates_instances;
            std::list<std::string> *input_encoding;
            virtual void DoRulesAdjustments(AspifLiteral *auxiliar_to_adjust);

        public:
            AspifRewritingReverser(std::list<AspifStatement*> &rules_set, std::unordered_map<int, AspifLiteral*> &_aux_predicates_instances, std::list<std::string> &_input_encoding);
            virtual int DoReverse();

    };
}

#endif