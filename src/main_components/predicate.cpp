#include "../../include/main_components/predicate.h"
using namespace aspsio;

Predicate::Predicate(const int &_id, const bool &_auxiliar):id(_id), auxiliar(_auxiliar), 
occurrences_in_bodies(0), occurrences_in_heads(0)
{
    
}

std::string Predicate::DoOutput(){

    std::string print = "";

    print += std::to_string(id) + " ";

    return print;
}