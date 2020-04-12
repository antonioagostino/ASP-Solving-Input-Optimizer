#ifndef ASPIF_MINIMIZE_STATEMENT
#define ASPIF_MINIMIZE_STATEMENT
#include "rule.h"

namespace aspsio {

    class AspifMinimizeStatement : public Rule {
        protected:
            int priority;

        public:
            AspifMinimizeStatement();
            void SetPriority(const int &value){ priority = value; }
            int GetPriority(){ return priority; }
            virtual void DoOutput() override;
    };

}

#endif