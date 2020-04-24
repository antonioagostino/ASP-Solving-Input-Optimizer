#include "../../include/main_components/aspif_statement.h"
using namespace aspsio;

AspifStatement::AspifStatement():aux_preds_in_head_number(0), aux_preds_in_body_number(0), deep_delete(true){}

AspifStatement::~AspifStatement(){
    
    *encoding_line = "";

    if(deep_delete){
        for (auto it = head.begin(); it != head.end(); it++)
        {
            delete (*it);
        }

        for (auto it = body.begin(); it != body.end(); it++)
        {
            delete (it->first);
            
            if(it->second != nullptr)
                delete (it->second);
        }
    }
    
}

void AspifStatement::AddInBody(AspifLiteral *literal){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(std::pair<AspifLiteral*, int*>(literal, nullptr));
}

void AspifStatement::AddInBody(AspifLiteral *literal,int *weight){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(std::pair<AspifLiteral*, int*>(literal, weight));
}

void AspifStatement::AddInHead(AspifLiteral *literal){
    if(literal->IsAuxiliar())
        aux_preds_in_head_number++;

    head.push_back(literal);
}

std::list<AspifLiteral*> AspifStatement::GetAuxiliarPredicatesInBody(){

    std::list<AspifLiteral*> auxiliars;
    for (auto it = body.begin(); it != body.end(); it++)
    {
        if((*it).first->IsAuxiliar())
            auxiliars.push_back(it->first);
    }

    return auxiliars;
    
}

std::list<AspifLiteral*> AspifStatement::GetAuxiliarPredicatesInHead(){

    std::list<AspifLiteral*> auxiliars;
    for (auto it = head.begin(); it != head.end(); it++)
    {
        if((*it)->IsAuxiliar())
            auxiliars.push_back(*it);
    }

    return auxiliars;
    
}

void AspifStatement::RemoveFromBody(AspifLiteral *literal){

    auto pos = body.begin();

    while (pos != body.end()) {
        if ((*pos).first == literal) 
            break;
        ++pos;
    }

    if(pos == body.end())
        return;

    if(literal->IsAuxiliar())
        aux_preds_in_body_number--;

    body.erase(pos);
}

void AspifStatement::RemoveFromHead(AspifLiteral *literal){
    auto pos = head.begin();

    while (pos != head.end()) {
        if ((*pos) == literal) 
            break;
        ++pos;
    }

    if(pos == head.end())
        return;

    if(literal->IsAuxiliar())
        aux_preds_in_head_number--;

    head.erase(pos);
}

std::pair<AspifLiteral*, int*> AspifStatement::FindALiteralInBody(AspifLiteral* val){
    
    auto pos = body.begin();

    while (pos != body.end()) {
        if ((*pos).first == val) 
            return *pos;
        ++pos;
    }
    
    return std::pair<AspifLiteral*, int*>(NULL, NULL);
}

AspifLiteral* AspifStatement::FindALiteralInHead(AspifLiteral* val){
    
    auto pos = head.begin();

    while (pos != head.end()) {
        if ((*pos) == val) 
            return *pos;
        ++pos;
    }

    return NULL;
}