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
            int lower_bound;

        public:
            AspifRuleStatement();
            void SetLowerBound(const int &value){ lower_bound = value; }
            void SetHeadType(HeadType type){ head_type = type; }
            void SetBodyType(BodyType type){ body_type = type; }
            int GetLowerBound(){ return lower_bound; }
            HeadType GetHeadType(){ return head_type; }
            BodyType GetBodyType(){ return body_type; }
            virtual void DoOutput() override;
    };

}

#endif