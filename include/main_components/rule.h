#ifndef RULE_H
#define RULE_H
#include <list>

namespace aspsio {

    class Rule {
        
        protected:
            std::list<int> body;
            std::list<int> head;
            int aux_pred_in_head;
            int aux_pred_in_body;
        public:
            Rule():aux_pred_in_body(0), aux_pred_in_head(0){}
            void AddInHead(const int &id, bool auxiliar_predicate)
            { 
                if(auxiliar_predicate)
                    aux_pred_in_head++;
                
                head.push_back(id); 
            }
            void AddInBody(const int &id, bool auxiliar_predicate)
            {
                if(auxiliar_predicate)
                    aux_pred_in_body++;
                
                body.push_back(id); 
            }

    };

}

#endif