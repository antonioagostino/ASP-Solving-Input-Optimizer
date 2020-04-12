#ifndef RULE_H
#define RULE_H
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include "weighted_predicate.h"

namespace aspsio {

    class Rule {
        
        protected:
            std::list<Predicate*> body;
            std::list<Predicate*> head;
            std::string *encoding_line;
            int aux_preds_in_body_number;
            int aux_preds_in_head_number;
            bool erasable;
            bool deep_delete;

        public:
            Rule();
            virtual void AddInHead(Predicate *predicate);
            virtual void AddInBody(Predicate *predicate);
            bool IsErasable(){ return erasable; }
            void SetErasable(const bool &state){ erasable = state; }
            int AuxiliarPredicatesInBodyNumber(){ return aux_preds_in_body_number; }
            int AuxiliarPredicatesInHeadNumber(){ return aux_preds_in_head_number; }
            std::list<Predicate*>& GetPredicatesInHead(){ return head; }
            std::list<Predicate*>& GetPredicatesInBody(){ return body; }
            std::list<Predicate*> GetAuxiliarPredicatesInHead();
            std::list<Predicate*> GetAuxiliarPredicatesInBody();
            bool IsDeepDelete(){ return deep_delete; }
            void SetDeepDelete(const bool &value){ deep_delete = value; }
            void SetEncodingLine(std::string &line){ encoding_line = &line; }
            virtual void DoOutput() = 0;
            ~Rule();
    };

}

#endif