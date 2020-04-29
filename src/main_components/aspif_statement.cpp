#include "../../include/main_components/aspif_statement.h"
using namespace aspsio;

AspifStatement::AspifStatement():aux_preds_in_head_number(0), aux_preds_in_body_number(0), useless(false){}

AspifStatement::~AspifStatement(){
    
    *encoding_line = "";
    
}

void AspifStatement::AddInBody(std::shared_ptr<AspifLiteral> literal){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>>(literal, nullptr));
}

void AspifStatement::AddInBody(std::shared_ptr<AspifLiteral> literal, std::shared_ptr<int> weight){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>>(literal, weight));
}

void AspifStatement::AddInHead(std::shared_ptr<AspifLiteral> literal){
    if(literal->IsAuxiliar())
        aux_preds_in_head_number++;

    head.push_back(literal);
}

std::list<std::shared_ptr<AspifLiteral>> AspifStatement::GetAuxiliarPredicatesInBody(){

    std::list<std::shared_ptr<AspifLiteral>> auxiliars;
    for (auto it = body.begin(); it != body.end(); it++)
    {
        if((*it).first->IsAuxiliar())
            auxiliars.push_back(it->first);
    }

    return auxiliars;
    
}

std::list<std::shared_ptr<AspifLiteral>> AspifStatement::GetAuxiliarPredicatesInHead(){

    std::list<std::shared_ptr<AspifLiteral>> auxiliars;
    for (auto it = head.begin(); it != head.end(); it++)
    {
        if((*it)->IsAuxiliar())
            auxiliars.push_back(*it);
    }

    return auxiliars;
    
}

void AspifStatement::RemoveFromBody(std::shared_ptr<AspifLiteral> literal){

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

void AspifStatement::RemoveFromHead(std::shared_ptr<AspifLiteral> literal){
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

std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>> AspifStatement::FindALiteralInBody(std::shared_ptr<AspifLiteral> val){
    
    auto pos = body.begin();

    while (pos != body.end()) {
        if ((*pos).first == val) 
            return *pos;
        ++pos;
    }
    
    return std::pair<std::shared_ptr<AspifLiteral>, std::shared_ptr<int>>(nullptr, nullptr);
}

std::shared_ptr<AspifLiteral> AspifStatement::FindALiteralInHead(std::shared_ptr<AspifLiteral> val){
    
    auto pos = head.begin();

    while (pos != head.end()) {
        if ((*pos) == val) 
            return *pos;
        ++pos;
    }

    return nullptr;
}