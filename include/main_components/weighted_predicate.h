#ifndef WEIGHTED_PREDICATE_H
#define WEIGHTED_PREDICATE_H
#include "predicate.h"

namespace aspsio {

    class WeightedPredicate : public Predicate
    {
        private:
            int weight;
        public:
            WeightedPredicate(const int &_id, const bool &_auxiliar, const int &_weight);
            virtual std::string DoOutput() override;
    };
    

}

#endif