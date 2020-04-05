#ifndef ASPIF_RULE_STATEMENT
#define ASPIF_RULE_STATEMENT
#include "rule.h"

namespace aspsio {

    enum HeadType {
        Disjunction = 0,
        Choice,
    };

    enum BodyType{
        NormalBody = 0,
        WeightBody,
    };

    class AspifRuleStatement : public Rule {
        protected:
            HeadType head_type;
            BodyType body_type;
            std::list<int> weights;
            int lower_bound;

        public:
            AspifRuleStatement();
            void SetLowerBound(const int &value){ lower_bound = value; }
            void AddWeight(const int &value){ weights.push_back(value); }
            void SetHeadType(HeadType type){ head_type = type; }
            void SetBodyType(BodyType type){ body_type = type; }
    };

}

#endif