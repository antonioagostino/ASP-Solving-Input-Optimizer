#ifndef ASPIF_MINIMIZE_STATEMENT
#define ASPIF_MINIMIZE_STATEMENT
#include "aspif_statement.h"

namespace aspsio {

    class AspifMinimizeStatement : public AspifStatement {
        protected:
            int priority;

        public:
            AspifMinimizeStatement();
            void SetPriority(const int &value){ priority = value; }
            int GetPriority(){ return priority; }
            virtual void DoOutput() override;
            virtual std::shared_ptr<AspifStatement> Clone();
    };

}

#endif