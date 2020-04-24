#ifndef ASPIF_STATEMENT_H
#define ASPIF_STATEMENT_H
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include "aspif_literal.h"

namespace aspsio {

    class AspifStatement {
        
        protected:
            std::list<AspifLiteral*> head;
            std::list<std::pair<AspifLiteral*, int*>> body;
            std::string *encoding_line;
            int aux_preds_in_body_number;
            int aux_preds_in_head_number;
            bool deep_delete;

        public:
            AspifStatement();
            virtual void AddInHead(AspifLiteral *literal);
            virtual void AddInBody(AspifLiteral *literal);
            virtual void AddInBody(AspifLiteral *literal, int *weight);
            virtual void RemoveFromBody(AspifLiteral *literal);
            virtual void RemoveFromHead(AspifLiteral *literal);
            int AuxiliarPredicatesInBodyNumber(){ return aux_preds_in_body_number; }
            int AuxiliarPredicatesInHeadNumber(){ return aux_preds_in_head_number; }
            std::list<AspifLiteral*>& GetPredicatesInHead(){ return head; }
            std::list<std::pair<AspifLiteral*, int*>>& GetPredicatesInBody(){ return body; }
            std::list<AspifLiteral*> GetAuxiliarPredicatesInHead();
            std::list<AspifLiteral*> GetAuxiliarPredicatesInBody();
            std::pair<AspifLiteral*, int*> FindALiteralInBody(AspifLiteral* val);
            AspifLiteral* FindALiteralInHead(AspifLiteral* val);
            bool IsDeepDelete(){ return deep_delete; }
            void SetDeepDelete(const bool &value){ deep_delete = value; }
            void SetEncodingLine(std::string &line){ encoding_line = &line; }
            std::string* GetEncodingLine(){ return encoding_line; }
            virtual void DoOutput() = 0;
            virtual AspifStatement* Clone() = 0;
            ~AspifStatement();
    };

}

#endif