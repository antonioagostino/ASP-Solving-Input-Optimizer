#ifndef ASPIF_MINIMIZE_STATEMENT
#define ASPIF_MINIMIZE_STATEMENT
#include "rule.h"

namespace aspsio {

    class AspifMinimizeStatement : public Rule {
        protected:
            std::list<int> weights;
            int priority;

        public:
            AspifMinimizeStatement();
            void SetPriority(const int &value){ priority = value; }
            void AddWeight(const int &value){ weights.push_back(value); }
    };

}

#endif