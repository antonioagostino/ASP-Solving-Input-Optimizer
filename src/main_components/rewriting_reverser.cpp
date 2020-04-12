#include "../../include/main_components/rewriting_reverser.h"
using namespace aspsio;

RewritingReverser::RewritingReverser(std::list<Rule*> &rules_set, std::unordered_map<int, Predicate*> &_aux_predicates_instances){
    rules_to_reverse = &rules_set;
    aux_predicates_instances = &_aux_predicates_instances;
}

//  Checks the occurrences of predicates in rules and performs a 
//  Reversing Algorithm depending on the number of occurrences in heads
//  or in bodies.

void RewritingReverser::DoReverse()
{

    auto it = aux_predicates_instances->begin();

    while (it != aux_predicates_instances->end())
    {
        Predicate *predicate_to_substitute = (*it).second;

        if(predicate_to_substitute->GetOccurrencesInHeads() == 1){
            
            std::list<Predicate*> *body;
            bool break_cond = false;
            std::list<Rule*>::iterator rule_to_delete;

            if(predicate_to_substitute->GetOccurrencesInBodies() == 1)
                break_cond = true;
            
            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                if((*rule)->AuxiliarPredicatesInBodyNumber() == 0 && (*rule)->AuxiliarPredicatesInHeadNumber() == 1){
                    std::list<Predicate*> auxs_in_head = (*rule)->GetAuxiliarPredicatesInHead();
                    if(std::find(auxs_in_head.begin(), auxs_in_head.end(), predicate_to_substitute) != auxs_in_head.end()){
                        body = &(*rule)->GetPredicatesInBody();
                        rule_to_delete = rule;
                        break;
                    }
                }
            }

            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                if((*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                    std::list<Predicate*> *auxs_in_body = &(*rule)->GetPredicatesInBody();
                    auto position = std::find(auxs_in_body->begin(), auxs_in_body->end(), predicate_to_substitute);
                    if(position != auxs_in_body->end()){
                        auxs_in_body->erase(position);
                        for (auto it = body->begin(); it != body->end(); it++)
                        {
                            auxs_in_body->push_back(*it);
                        }

                        if(break_cond)
                            break;
                    }
                }
            }

            delete (*rule_to_delete);
            rules_to_reverse->erase(rule_to_delete);
            it = aux_predicates_instances->erase(it);
        } else if(predicate_to_substitute->GetOccurrencesInHeads() > 1 && predicate_to_substitute->GetOccurrencesInBodies() == 1){

            std::list<Predicate*> *body;
            std::list<Predicate*> *head;
            std::list<Rule*>::iterator rule_to_delete;
            int occurrences_reversed = 0;
            int occurrences_to_reverse = predicate_to_substitute->GetOccurrencesInHeads();
            
            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                if((*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                    std::list<Predicate*> auxs_in_body = (*rule)->GetAuxiliarPredicatesInBody();
                    if(std::find(auxs_in_body.begin(), auxs_in_body.end(), predicate_to_substitute) != auxs_in_body.end()){
                        body = &(*rule)->GetPredicatesInBody();
                        head = &(*rule)->GetPredicatesInHead();
                        rule_to_delete = rule;
                        break;
                    }
                }
            }

            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                if((*rule)->AuxiliarPredicatesInBodyNumber() == 0 && (*rule)->AuxiliarPredicatesInHeadNumber() == 1){
                    std::list<Predicate*> auxs_in_head = (*rule)->GetAuxiliarPredicatesInHead();
                    if(std::find(auxs_in_head.begin(), auxs_in_head.end(), predicate_to_substitute) != auxs_in_head.end()){
                        (*rule)->GetPredicatesInHead().clear();
                        for (auto it = head->begin(); it != head->end(); it++)
                        {
                            (*rule)->GetPredicatesInHead().push_back(*it);
                        }

                        for (auto it = body->begin(); it != body->end(); it++)
                        {
                            if(*it != predicate_to_substitute)
                                (*rule)->GetPredicatesInBody().push_back(*it);
                        }

                        occurrences_reversed++;

                        if(occurrences_reversed == occurrences_to_reverse)
                            break;
                    }
                }
            }

            (*rule_to_delete)->SetDeepDelete(false);
            delete (*rule_to_delete);
            rules_to_reverse->erase(rule_to_delete);
            it = aux_predicates_instances->erase(it);
            delete predicate_to_substitute;
        } else {
            it++;
        }

    }
    
}

// Checks if there are rules with auxialiar predicates in its body

int RewritingReverser::RulesToOptimizeRemained(){
    
    int count = 0;
    for (auto it = rules_to_reverse->begin(); it != rules_to_reverse->end(); it++)
        if((*it)->AuxiliarPredicatesInBodyNumber() > 0)
            count++;

    return count;
    
}