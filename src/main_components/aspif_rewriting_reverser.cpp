#include "../../include/main_components/aspif_rewriting_reverser.h"
using namespace aspsio;

AspifRewritingReverser::AspifRewritingReverser(std::list<std::shared_ptr<AspifStatement>> &rules_set, std::unordered_map<int, std::shared_ptr<AspifLiteral>> &_aux_predicates_instances, std::list<std::string> &_input_encoding):duplicate_checking(true){
    rules_to_reverse = &rules_set;
    aux_predicates_instances = &_aux_predicates_instances;
    input_encoding = &_input_encoding;
}

//  Search a rule duplication, even if literals' order is different
//  then avoid a usefull duplication that may cause errors during
//  reversing

int AspifRewritingReverser::CountDuplicate(const std::string &rule){

    std::istringstream rule_stream(rule);

    int occurrences = 0;

    int rule_type, rule_second_param, literals_number, body_literals_number;;
    int body_type, lower_bound;

    std::vector<int> rule_head;
    std::vector<int> rule_body;

    rule_stream >> rule_type;
    rule_stream >> rule_second_param;
    rule_stream >> literals_number;

    for (int i = 0; i < literals_number; i++)
    {
        int literal;
        rule_stream >> literal;
        rule_head.push_back(literal);

        if(rule_type == 2){
            int weight;
            rule_stream >> weight;
            rule_head.push_back(weight);
        }
    }

    if(rule_type == 1){
        rule_stream >> body_type;
        if(body_type == 1)
            rule_stream >> lower_bound;
        
        rule_stream >> body_literals_number;

        for (int i = 0; i < body_literals_number; i++)
        {
            int literal;
            rule_stream >> literal;
            rule_body.push_back(literal);

            if(body_type == 1){
                int weight;
                rule_stream >> weight;
                rule_body.push_back(weight);
            }
        }
    }

    for(auto it = rules_to_reverse->begin();it != rules_to_reverse->end(); it++)
    {
        std::istringstream duplicate_stream(*((*it)->GetEncodingLine()));
        int duplicate_type, duplicate_second_param,  dupl_literals_number;
        std::vector<int> dupl_head;
        std::vector<int> dupl_body;

        duplicate_stream >> duplicate_type;

        if(rule_type != duplicate_type)
            continue;

        duplicate_stream >> duplicate_second_param;

        if(rule_second_param != duplicate_second_param)
            continue;

        duplicate_stream >> dupl_literals_number;
        
        if(dupl_literals_number != literals_number)
            continue;

        for (int i = 0; i < dupl_literals_number; i++)
        {
            int literal;
            duplicate_stream >> literal;
            dupl_head.push_back(literal);

            if(rule_type == 2){
                int weight;
                duplicate_stream >> weight;
                dupl_head.push_back(weight);
            }
        }

        bool equal_head = true;

        for (int idx = 0; idx < literals_number && equal_head; idx++)
        {
            bool found = false;

            for (int idx2 = 0; idx2 < dupl_literals_number && !found; idx2++)
            {
                if(rule_head[idx] != dupl_head[idx2]){
                    if(rule_type == 2)
                        idx2++;
                    continue;
                }

                if(rule_type == 2)
                    if(rule_head[idx + 1] != dupl_head[++idx2])
                        continue;

                found = true;
            }

            if(rule_type == 2)
                idx++;

            if(!found)
                equal_head = false;
        }

        if(!equal_head)
            continue;

        int dupl_body_type, dupl_lower_bound, dupl_literals_body_number;

        if(duplicate_type == 1){
            duplicate_stream >> dupl_body_type;

            if(dupl_body_type != body_type)
                continue;
            
            if(dupl_body_type == 1){
                duplicate_stream >> dupl_lower_bound;

                if(dupl_lower_bound != lower_bound)
                    continue;
            }

            duplicate_stream >> dupl_literals_body_number;
        

            if(dupl_literals_body_number != body_literals_number)
                continue;

            for (int i = 0; i < dupl_literals_body_number; i++)
            {
                int literal;
                duplicate_stream >> literal;
                dupl_body.push_back(literal);

                if(dupl_body_type == 1){
                    int weight;
                    duplicate_stream >> weight;
                    dupl_body.push_back(weight);
                }
            }

            bool equal_body = true;

            for (int idx = 0; idx < body_literals_number && equal_body; idx++)
            {
                bool found = false;

                for (int idx2 = 0; idx2 < dupl_literals_body_number && !found; idx2++)
                {
                    if(rule_body[idx] != dupl_body[idx2]){
                        if(body_type == 1)
                            idx2++;
                        continue;
                    }

                    if(body_type == 1)
                        if(rule_body[idx + 1] != dupl_body[++idx2])
                            continue;  

                    
                    found = true;
                }

                if(body_type == 1)
                    idx++;

                if(!found)
                    equal_body = false;
            }

            if(!equal_body)
                continue;

        }

        occurrences++;
        
    }


    return occurrences - 1;
}

//  Search a rule duplication, even if literals' order is different
//  then avoid a usefull duplication that may cause errors during
//  reversing

int AspifRewritingReverser::CountDuplicate(const std::string &rule, std::vector<std::string> &rules){

    std::istringstream rule_stream(rule);

    int occurrences = 0;

    int rule_type, rule_second_param, literals_number, body_literals_number;;
    int body_type, lower_bound;

    std::vector<int> rule_head;
    std::vector<int> rule_body;

    rule_stream >> rule_type;
    rule_stream >> rule_second_param;
    rule_stream >> literals_number;

    for (int i = 0; i < literals_number; i++)
    {
        int literal;
        rule_stream >> literal;
        rule_head.push_back(literal);

        if(rule_type == 2){
            int weight;
            rule_stream >> weight;
            rule_head.push_back(weight);
        }
    }

    if(rule_type == 1){
        rule_stream >> body_type;
        if(body_type == 1)
            rule_stream >> lower_bound;
        
        rule_stream >> body_literals_number;

        for (int i = 0; i < body_literals_number; i++)
        {
            int literal;
            rule_stream >> literal;
            rule_body.push_back(literal);

            if(body_type == 1){
                int weight;
                rule_stream >> weight;
                rule_body.push_back(weight);
            }
        }
    }

    for(auto it = rules.begin();it != rules.end(); it++)
    {
        std::istringstream duplicate_stream(*it);
        int duplicate_type, duplicate_second_param,  dupl_literals_number;
        std::vector<int> dupl_head;
        std::vector<int> dupl_body;

        duplicate_stream >> duplicate_type;

        if(rule_type != duplicate_type)
            continue;

        duplicate_stream >> duplicate_second_param;

        if(rule_second_param != duplicate_second_param)
            continue;

        duplicate_stream >> dupl_literals_number;
        
        if(dupl_literals_number != literals_number)
            continue;

        for (int i = 0; i < dupl_literals_number; i++)
        {
            int literal;
            duplicate_stream >> literal;
            dupl_head.push_back(literal);

            if(rule_type == 2){
                int weight;
                duplicate_stream >> weight;
                dupl_head.push_back(weight);
            }
        }

        bool equal_head = true;

        for (int idx = 0; idx < literals_number && equal_head; idx++)
        {
            bool found = false;

            for (int idx2 = 0; idx2 < dupl_literals_number && !found; idx2++)
            {
                if(rule_head[idx] != dupl_head[idx2]){
                    if(rule_type == 2)
                        idx2++;
                    continue;
                }

                if(rule_type == 2)
                    if(rule_head[idx + 1] != dupl_head[++idx2])
                        continue;

                found = true;
            }

            if(rule_type == 2)
                idx++;

            if(!found)
                equal_head = false;
        }

        if(!equal_head)
            continue;

        int dupl_body_type, dupl_lower_bound, dupl_literals_body_number;

        if(duplicate_type == 1){
            duplicate_stream >> dupl_body_type;

            if(dupl_body_type != body_type)
                continue;
            
            if(dupl_body_type == 1){
                duplicate_stream >> dupl_lower_bound;

                if(dupl_lower_bound != lower_bound)
                    continue;
            }

            duplicate_stream >> dupl_literals_body_number;
        

            if(dupl_literals_body_number != body_literals_number)
                continue;

            for (int i = 0; i < dupl_literals_body_number; i++)
            {
                int literal;
                duplicate_stream >> literal;
                dupl_body.push_back(literal);

                if(dupl_body_type == 1){
                    int weight;
                    duplicate_stream >> weight;
                    dupl_body.push_back(weight);
                }
            }

            bool equal_body = true;

            for (int idx = 0; idx < body_literals_number && equal_body; idx++)
            {
                bool found = false;

                for (int idx2 = 0; idx2 < dupl_literals_body_number && !found; idx2++)
                {
                    if(rule_body[idx] != dupl_body[idx2]){
                        if(body_type == 1)
                            idx2++;
                        continue;
                    }

                    if(body_type == 1)
                        if(rule_body[idx + 1] != dupl_body[++idx2])
                            continue;  

                    
                    found = true;
                }

                if(body_type == 1)
                    idx++;

                if(!found)
                    equal_body = false;
            }

            if(!equal_body)
                continue;

        }

        occurrences++;
        
    }

    return occurrences;
}


//  Checks if it's necessary to duplicate rules
//  to performs a right Reversing of these rules

void AspifRewritingReverser::DoRulesAdjustments(std::shared_ptr<AspifLiteral> auxiliar_to_adjust){

    // We need to duplicate some rules to generate a complete reversing

    if(auxiliar_to_adjust->GetOccurrencesInHeads() > 1 && auxiliar_to_adjust->GetOccurrencesInBodies() > 0){
        std::list<std::string>::iterator where_to_insert;
        std::vector<std::string> duplicated;

        for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
        {
            int occurrences_in_heads = auxiliar_to_adjust->GetOccurrencesInHeads();
            if((*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                AspifRuleLiteral position = (*rule)->FindALiteralInBody(auxiliar_to_adjust);
                if(position.GetLiteral() && !duplicate_checking || position.GetLiteral() && CountDuplicate(*((*rule)->GetEncodingLine()), duplicated) == 0){
                    
                    if(duplicate_checking){
                        int rule_duplication = CountDuplicate(*((*rule)->GetEncodingLine()));

                        //  If false, there will no be duplication
                        if(rule_duplication < (occurrences_in_heads - 1)){
                            where_to_insert = find(input_encoding->begin(), input_encoding->end(), *(*rule)->GetEncodingLine());
                            duplicated.push_back(*((*rule)->GetEncodingLine()));
                        }

                        occurrences_in_heads -= rule_duplication;
                    } else {
                        where_to_insert = find(input_encoding->begin(), input_encoding->end(), *(*rule)->GetEncodingLine());
                    }

                    
                    //  Duplicates, n - 1 times, rules where this auxiliar predicates occurs in body
                    //  where "n" is the occurrences number in rules' head

                    for (int i = 1; i < occurrences_in_heads; i++)
                    {
                        std::shared_ptr<AspifStatement> new_rule = (*rule)->Clone();
                        if(where_to_insert != input_encoding->end()){
                            where_to_insert = input_encoding->insert(where_to_insert, *where_to_insert);
                        }

                        new_rule->SetEncodingLine(*where_to_insert);
                        rule = rules_to_reverse->insert(++rule, new_rule);

                        //  We must increment the occurrences of auxiliar predicates in bodies or
                        //  heads to control reversing of rules containing it
                        for (auto it = new_rule->GetPredicatesInBody().begin(); it != new_rule->GetPredicatesInBody().end(); it++)
                        {
                            it->GetLiteral()->IncrementOccurrencesInBodies();
                        }

                        for (auto it = new_rule->GetPredicatesInHead().begin(); it != new_rule->GetPredicatesInHead().end(); it++)
                        {
                            it->GetLiteral()->IncrementOccurrencesInHeads();
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
        std::shared_ptr<AspifLiteral> predicate_to_substitute = (*it).second;
        bool aux_reversed = false;
        bool auxiliar_adjusted = false;
        bool auxiliar_will_be_reversed_later = false;

        //  An auxiliar predicate could have a dependence by another auxiliare predicate
        //  in a rule's body BUT could haven't in other rules. So an auxiliare predicate
        //  can be reversed only if it has not any dependence.

        for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
        {
            if((*rule)->AuxiliarPredicatesInHeadNumber() == 1){
                AspifRuleLiteral aux_in_head = (*rule)->FindALiteralInHead(predicate_to_substitute);
                if(aux_in_head.GetLiteral() && (*rule)->AuxiliarPredicatesInBodyNumber() > 0){
                    auxiliar_will_be_reversed_later = true;
                    break;
                }
            }
        }

        // Substitution of rule's bodies containing auxiliar predicates with
        // rules' body where auxiliar predicates appear in head

        //  Reversing Step 1: Checks if a rule hasn't auxiliar predicates in 
        //  its body, and it HAS an auxiliar predicate in its head.
        //  This step is repeated for each auxiliar predicate's occurrence
        //  in head.

        for (int i = 0; i < predicate_to_substitute->GetOccurrencesInHeads() && !auxiliar_will_be_reversed_later; i++)
        {
            std::list<AspifRuleLiteral> *body = NULL;
            std::list<std::shared_ptr<AspifStatement>>::iterator rule_to_delete = rules_to_reverse->end();
            bool rule_can_be_eliminated = false;
            
            for (auto rule = rules_to_reverse->begin(); rule != rules_to_reverse->end(); rule++)
            {
                
                if((*rule)->AuxiliarPredicatesInBodyNumber() == 0 && (*rule)->AuxiliarPredicatesInHeadNumber() == 1){
                    AspifRuleLiteral aux_in_head = (*rule)->FindALiteralInHead(predicate_to_substitute);
                    if(aux_in_head.GetLiteral()){
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

                        //  First condition: Due to I-DLV rules duplication
                        if(predicate_to_substitute->GetOccurrencesInHeads() == 1 || CountDuplicate(*((*rule)->GetEncodingLine()), rules_reversed) == 0){
                            AspifRuleLiteral position = (*rule)->FindALiteralInBody(predicate_to_substitute);
                            if(position.GetLiteral()){
                                
                                (*rule)->RemoveFromBody(position.GetLiteral());
                                for (auto it = body->begin(); it != body->end(); it++)
                                {
                                    if(!it->HasWeight() && position.HasWeight())
                                        it->SetWeight(position.GetWeight());
                                    
                                    bool positive = it->IsPositive();

                                    if(!position.IsPositive())
                                        positive = !positive;
                                    
                                    if(it->HasWeight())
                                        (*rule)->AddInBody(it->GetLiteral(), it->GetWeight(), positive);
                                    else
                                        (*rule)->AddInBody(it->GetLiteral(), positive);
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
                (*rule_to_delete)->SetUselessness(true);
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