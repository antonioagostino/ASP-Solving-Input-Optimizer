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
            std::list<std::shared_ptr<AspifStatement>> *rules_to_reverse;
            std::unordered_map<int, std::shared_ptr<AspifLiteral>> *aux_predicates_instances;
            std::list<std::string> *input_encoding;
            virtual void DoRulesAdjustments(std::shared_ptr<AspifLiteral> auxiliar_to_adjust);

        public:
            AspifRewritingReverser(std::list<std::shared_ptr<AspifStatement>> &rules_set, std::unordered_map<int, std::shared_ptr<AspifLiteral>> &_aux_predicates_instances, std::list<std::string> &_input_encoding);
            virtual int DoReverse();

    };
}

#endif