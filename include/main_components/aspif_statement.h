#ifndef ASPIF_STATEMENT_H
#define ASPIF_STATEMENT_H
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
#include <math.h>
#include "aspif_literal.h"

namespace aspsio {

    class AspifStatement {
        
        protected:
            std::list<std::shared_ptr<AspifLiteral>> head;
            std::list<std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>>> body;
            std::string *encoding_line;
            int aux_preds_in_body_number;
            int aux_preds_in_head_number;
            bool useless;

        public:
            AspifStatement();
            virtual void AddInHead(std::shared_ptr<AspifLiteral> literal);
            virtual void AddInBody(std::shared_ptr<AspifLiteral> literal);
            virtual void AddInBody(std::shared_ptr<AspifLiteral> literal, std::shared_ptr<int> weight);
            virtual void RemoveFromBody(std::shared_ptr<AspifLiteral> literal);
            virtual void RemoveFromHead(std::shared_ptr<AspifLiteral> literal);
            int AuxiliarPredicatesInBodyNumber(){ return aux_preds_in_body_number; }
            int AuxiliarPredicatesInHeadNumber(){ return aux_preds_in_head_number; }
            std::list<std::shared_ptr<AspifLiteral>>& GetPredicatesInHead(){ return head; }
            std::list<std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>>>& GetPredicatesInBody(){ return body; }
            std::list<std::shared_ptr<AspifLiteral>> GetAuxiliarPredicatesInHead();
            std::list<std::shared_ptr<AspifLiteral>> GetAuxiliarPredicatesInBody();
            std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>> FindALiteralInBody(std::shared_ptr<AspifLiteral> val);
            std::shared_ptr<AspifLiteral> FindALiteralInHead(std::shared_ptr<AspifLiteral> val);
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