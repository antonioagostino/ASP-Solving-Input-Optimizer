#ifndef ASPIF_STATEMENT_H
#define ASPIF_STATEMENT_H
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
#include <math.h>
#include "aspif_rule_literal.h"

namespace aspsio {

    class AspifStatement {
        
        protected:
            std::list<AspifRuleLiteral> head;
            std::list<AspifRuleLiteral> body;
            std::string *encoding_line;
            int aux_preds_in_body_number;
            int aux_preds_in_head_number;
            bool useless;

        public:
            AspifStatement();
            virtual void AddInHead(std::shared_ptr<AspifLiteral> literal);
            virtual void AddInBody(std::shared_ptr<AspifLiteral> literal, const bool &positive_literal);
            virtual void AddInBody(std::shared_ptr<AspifLiteral> literal, const int &weight, const bool &positive_literal);
            virtual void RemoveFromBody(std::shared_ptr<AspifLiteral> literal);
            virtual void RemoveFromHead(std::shared_ptr<AspifLiteral> literal);
            int AuxiliarPredicatesInBodyNumber(){ return aux_preds_in_body_number; }
            int AuxiliarPredicatesInHeadNumber(){ return aux_preds_in_head_number; }
            std::list<AspifRuleLiteral>& GetPredicatesInHead(){ return head; }
            std::list<AspifRuleLiteral>& GetPredicatesInBody(){ return body; }
            std::list<std::shared_ptr<AspifLiteral>> GetAuxiliarPredicatesInHead();
            std::list<std::shared_ptr<AspifLiteral>> GetAuxiliarPredicatesInBody();
            AspifRuleLiteral FindALiteralInBody(std::shared_ptr<AspifLiteral> val);
            AspifRuleLiteral FindALiteralInHead(std::shared_ptr<AspifLiteral> val);
            bool IsUseless(){ return useless; }
            void SetUselessness(const bool &value){ useless = value; }
            void SetEncodingLine(std::string &line){ encoding_line = &line; }
            std::string* GetEncodingLine(){ return encoding_line; }
            virtual void DoOutput() = 0;
            virtual std::shared_ptr<AspifStatement> Clone() = 0;
            ~AspifStatement();
    };

}

#endif