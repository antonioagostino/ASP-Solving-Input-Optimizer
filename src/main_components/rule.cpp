#include "../../include/main_components/rule.h"
using namespace aspsio;

Rule::Rule():aux_preds_in_head_number(0), aux_preds_in_body_number(0), erasable(false), deep_delete(true){}

Rule::~Rule(){
    
    *encoding_line = "";

    if(deep_delete){
        for (auto it = head.begin(); it != head.end(); it++)
        {
            delete (*it);
        }

        for (auto it = body.begin(); it != body.end(); it++)
        {
            delete (*it);
        }
    }
    
}

void Rule::AddInBody(Predicate *predicate){ 

    if(predicate->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(predicate);
}

void Rule::AddInHead(Predicate *predicate){
    if(predicate->IsAuxiliar())
        aux_preds_in_head_number++;

    head.push_back(predicate);
}

std::list<Predicate*> Rule::GetAuxiliarPredicatesInBody(){

    std::list<Predicate*> auxiliars;
    for (auto it = body.begin(); it != body.end(); it++)
    {
        if((*it)->IsAuxiliar())
            auxiliars.push_back(*it);
    }

    return auxiliars;
    
}

std::list<Predicate*> Rule::GetAuxiliarPredicatesInHead(){

    std::list<Predicate*> auxiliars;
    for (auto it = head.begin(); it != head.end(); it++)
    {
        if((*it)->IsAuxiliar())
            auxiliars.push_back(*it);
    }

    return auxiliars;
    
}