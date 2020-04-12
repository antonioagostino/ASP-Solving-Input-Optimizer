#include "../../include/main_components/weighted_predicate.h"
using namespace aspsio;

WeightedPredicate::WeightedPredicate(const int &_id, const bool &_auxiliar, const int &_weight):Predicate(_id, _auxiliar), weight(weight){

}

std::string WeightedPredicate::DoOutput(){
    std::string print = "";

    print += std::to_string(id) + " " + std::to_string(weight) + " ";

    return print;
}