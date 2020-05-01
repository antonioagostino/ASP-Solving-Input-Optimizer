#ifndef ASPIF_RULE_LITERAL
#define ASPIF_RULE_LITERAL
#include <memory>
#include "aspif_literal.h"
namespace aspsio {
    
    class AspifRuleLiteral {

        private:
            std::shared_ptr<AspifLiteral> literal;
            int weight;
            bool positive_literal;
            bool has_weight;
        public:
            AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal, const int &_weight, const bool &_positive);
            AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal, const bool &_positive);
            AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal);
            bool HasWeight(){ return has_weight; }
            int GetWeight(){ return weight; }
            void SetWeight(const int &value){ weight = value; has_weight = true;}
            bool IsPositive(){ return positive_literal; }
            void SetPositive(const bool &value){ positive_literal = value; }
            std::shared_ptr<AspifLiteral> GetLiteral(){ return literal; }
    };
}

#endif