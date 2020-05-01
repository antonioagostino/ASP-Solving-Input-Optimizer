#include "../../include/main_components/aspif_statement.h"
using namespace aspsio;

AspifStatement::AspifStatement():aux_preds_in_head_number(0), aux_preds_in_body_number(0), useless(false){}

AspifStatement::~AspifStatement(){
    
    *encoding_line = "";
    
}

void AspifStatement::AddInBody(std::shared_ptr<AspifLiteral> literal, const bool &positive_literal){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(AspifRuleLiteral(literal, positive_literal));
}

void AspifStatement::AddInBody(std::shared_ptr<AspifLiteral> literal, const int &weight, const bool &positive_literal){ 

    if(literal->IsAuxiliar())
        aux_preds_in_body_number++;

    body.push_back(AspifRuleLiteral(literal, weight, positive_literal));
}

void AspifStatement::AddInHead(std::shared_ptr<AspifLiteral> literal){
    if(literal->IsAuxiliar())
        aux_preds_in_head_number++;

    head.push_back(AspifRuleLiteral(literal));
}

std::list<std::shared_ptr<AspifLiteral>> AspifStatement::GetAuxiliarPredicatesInBody(){

    std::list<std::shared_ptr<AspifLiteral>> auxiliars;
    for (auto it = body.begin(); it != body.end(); it++)
    {
        if((*it).GetLiteral()->IsAuxiliar())
            auxiliars.push_back(it->GetLiteral());
    }

    return auxiliars;
    
}

std::list<std::shared_ptr<AspifLiteral>> AspifStatement::GetAuxiliarPredicatesInHead(){

    std::list<std::shared_ptr<AspifLiteral>> auxiliars;
    for (auto it = head.begin(); it != head.end(); it++)
    {
        if((*it).GetLiteral()->IsAuxiliar())
            auxiliars.push_back(it->GetLiteral());
    }

    return auxiliars;
    
}

void AspifStatement::RemoveFromBody(std::shared_ptr<AspifLiteral> literal){

    auto pos = body.begin();

    while (pos != body.end()) {
        if ((*pos).GetLiteral() == literal) 
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
        if ((*pos).GetLiteral() == literal) 
            break;
        ++pos;
    }

    if(pos == head.end())
        return;

    if(literal->IsAuxiliar())
        aux_preds_in_head_number--;

    head.erase(pos);
}

AspifRuleLiteral AspifStatement::FindALiteralInBody(std::shared_ptr<AspifLiteral> val){
    
    auto pos = body.begin();

    while (pos != body.end()) {
        if ((*pos).GetLiteral() == val) 
            return *pos;
        ++pos;
    }
    
    return AspifRuleLiteral(nullptr);
}

AspifRuleLiteral AspifStatement::FindALiteralInHead(std::shared_ptr<AspifLiteral> val){
    
    auto pos = head.begin();

    while (pos != head.end()) {
        if ((*pos).GetLiteral() == val) 
            return *pos;
        ++pos;
    }

    return AspifRuleLiteral(nullptr);
}