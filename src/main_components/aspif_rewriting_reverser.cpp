#include "../../include/main_components/aspif_rewriting_reverser.h"
using namespace aspsio;

AspifRewritingReverser::AspifRewritingReverser(std::list<AspifStatement*> &rules_set, std::unordered_map<int, AspifLiteral*> &_aux_predicates_instances, std::list<std::string> &_input_encoding){
    rules_to_reverse = &rules_set;
    aux_predicates_instances = &_aux_predicates_instances;
    input_encoding = &_input_encoding;
}


//  Checks if it's necessary to duplicate rules
//  to performs a right Reversing of these rules

void AspifRewritingReverser::DoRulesAdjustments(AspifLiteral *auxiliar_to_adjust){

    // We need to duplicate some rules to generate a complete reversing

    if(auxiliar_to_adjust->GetOccurrencesInHeads() > 1 && auxiliar_to_adjust->GetOccurrencesInBodies() > 0){
        int occurrences_in_heads = auxiliar_to_adjust->GetOccurrencesInHeads();
        std::list<std::string>::iterator where_to_insert;

        for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
        {
            if((*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                std::list<std::pair<AspifLiteral*, int*>> *auxs_in_body = &(*rule)->GetPredicatesInBody();
                std::pair<AspifLiteral*, int*> position = (*rule)->FindALiteralInBody(auxiliar_to_adjust);
                if(position.first != NULL){
                    where_to_insert = find(input_encoding->begin(), input_encoding->end(), *(*rule)->GetEncodingLine());
                    
                    //  Duplicates, n - 1 times, rules where this auxiliar predicates occurs in body
                    //  where "n" is the occurrences number in rules' head

                    for (int i = 1; i < occurrences_in_heads; i++)
                    {
                        AspifStatement* new_rule = (*rule)->Clone();
                        if(where_to_insert != input_encoding->end()){
                            where_to_insert = input_encoding->insert(where_to_insert, *where_to_insert);
                        }

                        new_rule->SetEncodingLine(*where_to_insert);
                        rule = rules_to_reverse->insert(++rule, new_rule);

                        //  We must increment the occurrences of auxiliar predicates in bodies or
                        //  heads to control reversing of rules containing it
                        for (auto it = new_rule->GetPredicatesInBody().begin(); it != new_rule->GetPredicatesInBody().end(); it++)
                        {
                            (*it).first->IncrementOccurrencesInBodies();
                        }

                        for (auto it = new_rule->GetPredicatesInHead().begin(); it != new_rule->GetPredicatesInHead().end(); it++)
                        {
                            (*it)->IncrementOccurrencesInHeads();
                        }
                        
                    }
                    
                }
            }
        }
    }
}

//  Checks the occurrences of predicates in rules and performs a 
//  Reversing Algorithm depending on the number of occurrences in heads
//  or in bodies.

int AspifRewritingReverser::DoReverse()
{

    int reverses_done = 0;
    auto it = aux_predicates_instances->begin();
    
    while (it != aux_predicates_instances->end() && rules_to_reverse->size() > 0)
    {
        AspifLiteral *predicate_to_substitute = (*it).second;
        bool aux_reversed = false;
        bool auxiliar_adjusted = false;

        // Substitution of rule's bodies containing auxiliar predicates with
        // rules' body where auxiliar predicates appear in head

        //  Reversing Step 1: Checks if a rule hasn't auxiliar predicates in 
        //  its body, and it HAS an auxiliar predicate in its head.
        //  This step is repeated for each auxiliar predicate's occurrence
        //  in head.

        for (int i = 0; i < predicate_to_substitute->GetOccurrencesInHeads(); i++)
        {
            std::list<std::pair<AspifLiteral*, int*>> *body = NULL;
            std::list<AspifStatement*>::iterator rule_to_delete = rules_to_reverse->end();
            bool rule_can_be_eliminated = false;
            
            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                
                if((*rule)->AuxiliarPredicatesInBodyNumber() == 0 && (*rule)->AuxiliarPredicatesInHeadNumber() == 1){
                    AspifLiteral* aux_in_head = (*rule)->FindALiteralInHead(predicate_to_substitute);
                    if(aux_in_head != NULL){
                        body = &(*rule)->GetPredicatesInBody();
                        rule_to_delete = rule;
                        break;
                    }
                }
            }

            //  After saving the body of the rule reversing needs, it performs a
            //  simple substitution of body literals. If it involves weight bodies,
            //  it manages them.
            //  IMPORTANT: Auxiliar Predicates' occurrences, in bodies and in heads,
            //  are necessary to calculets how many substitutions performs.

            if(predicate_to_substitute->GetOccurrencesInBodies() > 0 && body != NULL){

                if(!auxiliar_adjusted){
                    auxiliar_adjusted = true;
                    DoRulesAdjustments(predicate_to_substitute);
                }

                int reverses_limit = predicate_to_substitute->GetOccurrencesInBodies() / predicate_to_substitute->GetOccurrencesInHeads();

                std::vector<std::string> rules_reversed;
                for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
                {
                    if((*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                        if(find(rules_reversed.begin(), rules_reversed.end(), *(*rule)->GetEncodingLine()) == rules_reversed.end()){
                            std::list<std::pair<AspifLiteral*, int*>> *auxs_in_body = &(*rule)->GetPredicatesInBody();
                            std::pair<AspifLiteral*, int*> position = (*rule)->FindALiteralInBody(predicate_to_substitute);
                            if(position.first != NULL){

                                int *weight = NULL;
                                if(position.second != NULL)
                                    weight = position.second;
                                
                                (*rule)->RemoveFromBody(position.first);
                                for (auto it = body->begin(); it != body->end(); it++)
                                {
                                    if(it->second == NULL && weight != NULL)
                                        it->second = weight;
                                    
                                    (*rule)->AddInBody(it->first, it->second);
                                }

                                rules_reversed.push_back(*(*rule)->GetEncodingLine());
                                (*rule)->DoOutput();
                                aux_reversed = true;
                                reverses_done++;

                                if(rules_reversed.size() >= reverses_limit){
                                    rule_can_be_eliminated = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else if(predicate_to_substitute->GetOccurrencesInBodies() == 0){

                //  A rule without a possible substitution is useless
                //  and an auxiliar predicate is useless too
                rule_can_be_eliminated = true;
                aux_reversed = true;
            }

            //  Rule removing to save computation steps
            if(rule_to_delete != rules_to_reverse->end() && rule_can_be_eliminated){
                (*rule_to_delete)->SetDeepDelete(false);
                delete (*rule_to_delete);
                rules_to_reverse->erase(rule_to_delete);
            }
        }

        if(aux_reversed){
            it = aux_predicates_instances->erase(it);
        } else {
            it++;
        }

    }

    return reverses_done;
    
}