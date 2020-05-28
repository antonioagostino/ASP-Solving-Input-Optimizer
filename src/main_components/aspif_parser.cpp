#include "../../include/main_components/aspif_parser.h"
using namespace aspsio;

AspifParser::AspifParser(std::list<std::string> &input_data, std::vector<std::list<std::shared_ptr<AspifStatement>>> &rules_sets, 
                    std::vector<std::string> &pattern_set):optimize_reversing_activated(false)
{
    input_encoding = &input_data;
    rules_to_optimize = &rules_sets;
    patterns_to_match = &pattern_set;
}

//  Adds a new Rewriting Pattern that auxiliar precicates have to match

void AspifParser::AddReverseParsingOption(const std::string &pattern){
    rules_to_optimize->push_back(std::list<std::shared_ptr<AspifStatement>>());
    patterns_to_match->push_back(pattern);
    aux_predicates_instances.push_back(std::unordered_map<int, std::shared_ptr<AspifLiteral>>());
}

void AspifParser::StartAnalysis(){
    SaveAuxiliarPredicates();
    SaveRulesToOptimize();
}

// Controls "output statements" of the aspif format
// which usually are placed at the end of the encoded file.
// Then it saves them in a data structure, so they can be used later.

void AspifParser::SaveAuxiliarPredicates(){
    
    line_to_parse = input_encoding->end();

    //Avoiding the parsing of last line "0"
    for (int i = 0; i < 2; i++)
        line_to_parse--;

    bool show_section_ended = false;

    while (!show_section_ended)
    {
        int statement_type, name_lenght, predicate_id, condition_lenght;
        std::string predicate_name;

        std::istringstream sstream(*line_to_parse);
        sstream >> statement_type >> name_lenght >> predicate_name >> condition_lenght >> predicate_id;

        //  Statement Type "4" represents Aspif Output Statement
        if(statement_type == 4){
            
            //  If the auxiliar predicate isn't a fact
            if(condition_lenght > 0){

                //  It checks if auxiliar predicate's name matches a rewriting pattern
                for (int i = 0; i < patterns_to_match->size(); i++)
                {
                    if(predicate_name.substr(0, patterns_to_match->at(i).size()) == patterns_to_match->at(i)){

                        // The first element is the predicate's id
                        // The second element is the type of Rewriting
                        aux_predicates.insert(std::pair<int, int>(predicate_id, i));
                        *line_to_parse = "";

                        break;
                    }

                }
            }

            line_to_parse--;
        } else {
            show_section_ended = true;
        }
        
    }

    //  To avoid jumping last line
    line_to_parse++;

}

// Checks if a rule contains an auxiliar predicate, if it's true
// save an abstraction of this rule in a data structure

void AspifParser::SaveRulesToOptimize(){

    auto it = input_encoding->begin();

    //We can avoid to parse the aspif header (so we start from index 1)
    it++;

    for (; it != line_to_parse; it++)
    {
        int statement_type = std::stoi((*it).substr(0, 1));

        if(statement_type == 1){ //We are parsing a rule statement
            ParseRuleStatement((*it).substr(2), (*it));
        } else if(statement_type == 2){ //We are parsing a minimize statement
            ParseMinimizeStatement((*it).substr(2), (*it));
        }

    }
}

// It parses an Aspif Rule Statement to check if it contains auxiliar predicates
// if it's true, calls a method to store it

void AspifParser::ParseRuleStatement(std::string input_line, std::string &full_input_line){

    bool rule_must_be_optimized = false;
    int head_type;
    int head_size;
    std::istringstream sstream(input_line);

    sstream >> head_type >> head_size;

    for (int x = 0; x < head_size && !rule_must_be_optimized; x++)
    {
        int predicate_id;
        sstream >> predicate_id;
        for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
        {
            if(std::abs(predicate_id) == j->first){
                rule_must_be_optimized = true;
                break;
            }
        }
        
    }
    
    if(rule_must_be_optimized){
        StoreSingleRuleStatement(input_line.substr(2), full_input_line, head_type);
    } else {

        int body_type, body_size, lower_bound;

        sstream >> body_type;

        if(body_type == NormalBody){

            sstream >> body_size;

        } else if(body_type == WeightBody){

            sstream >> lower_bound;
            sstream >> body_size;

        }

        for (int i = 0; i < body_size && !rule_must_be_optimized; i++)
        {
            int predicate_id;
            bool is_an_auxiliar_predicate = false;

            sstream >> predicate_id;
            for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
            {
                if(std::abs(predicate_id) == j->first){
                    rule_must_be_optimized = true;
                    break;
                }
            }

            if(body_type == WeightBody){
                int weight;
                sstream >> weight;
            }

        }

        if(rule_must_be_optimized)
            StoreSingleRuleStatement(input_line.substr(2), full_input_line, head_type);
        
    }
}

// It parses an Aspif Minimize Statement to check if it contains auxiliar predicates
// if it's true, calls a method to store it

void AspifParser::ParseMinimizeStatement(std::string input_line, std::string &full_input_line){

    bool rule_must_be_optimized = false;
    std::istringstream sstream(input_line);
    int priority, literals_number;

    sstream >> priority;
    sstream >> literals_number;

    for (int i = 0; i < literals_number && !rule_must_be_optimized; i++)
    {
        int predicate_id;

        sstream >> predicate_id;
        for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
        {
            if(std::abs(predicate_id) == j->first){
                rule_must_be_optimized = true;
                break;
            }
        }
    }

    if(rule_must_be_optimized)
        StoreSingleMinimizeStatement(input_line.substr(2), full_input_line, priority);
    

}

// Stores a single Rule Statement of the aspif format, which before was analyzed
// and that analysis proves that it contains auxiliar predicates

void AspifParser::StoreSingleRuleStatement(std::string input_line, std::string &full_input_line, const int &head_type){
    
    std::istringstream sstream(input_line);
    std::shared_ptr<AspifRuleStatement> rule (new AspifRuleStatement());
    HeadType head_t;

    if(head_type == 0)
        head_t = Disjunction;
    else if(head_type == 1)
        head_t = Choice;
    
    rule->SetHeadType(head_t);
    rule->SetEncodingLine(full_input_line);
    int head_size;
    bool head_aux_found = false;
    sstream >> head_size;

    for (int i = 0; i < head_size; i++)
    {
        int predicate_id;
        bool is_an_auxiliar_predicate = false;
        int rewriting_type_id = -1;

        sstream >> predicate_id;
        if(!head_aux_found){

            for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
            {
                if(std::abs(predicate_id) == j->first){
                    is_an_auxiliar_predicate = true;
                    head_aux_found = true;
                    rewriting_type_id = j->second;
                    if(find(rules_to_optimize->at(rewriting_type_id).begin(), rules_to_optimize->at(rewriting_type_id).end(), rule) == rules_to_optimize->at(rewriting_type_id).end())
                        rules_to_optimize->at(rewriting_type_id).push_back(rule);
                    break;
                }
            }

        }

        std::shared_ptr<AspifLiteral> predicate;

        if(is_an_auxiliar_predicate){

            auto element = aux_predicates_instances[rewriting_type_id].find(std::abs(predicate_id));

            if(element == aux_predicates_instances[rewriting_type_id].end()){
                predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
                aux_predicates_instances[rewriting_type_id].insert(std::pair<int, std::shared_ptr<AspifLiteral>>(std::abs(predicate_id), predicate));
                
            } else {
                predicate = element->second;
            }

            // It helps to save computation time later, during the Reversing Phase
            predicate->IncrementOccurrencesInHeads();

        } else {
            predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
        }

        //  Literals in head can't be negative
        rule->AddInHead(predicate);

    }

    int body_type, body_size, lower_bound;
    BodyType body_t;

    sstream >> body_type;

    if(body_type == NormalBody){
        
        // The lower bound parameter will not be used
        lower_bound = -1;
        body_t = NormalBody;

        sstream >> body_size;

    } else if(body_type == WeightBody){

        body_t = WeightBody;
        sstream >> lower_bound;
        sstream >> body_size;

    }

    rule->SetBodyType(body_t);
    rule->SetLowerBound(lower_bound);

    for (int i = 0; i < body_size; i++)
    {
        int predicate_id;
        bool is_an_auxiliar_predicate = false;
        int rewriting_type_id = -1;
        sstream >> predicate_id;
        for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
        {
            if(std::abs(predicate_id) == j->first){
                is_an_auxiliar_predicate = true;
                rewriting_type_id = j->second;
                if(find(rules_to_optimize->at(rewriting_type_id).begin(), rules_to_optimize->at(rewriting_type_id).end(), rule) == rules_to_optimize->at(rewriting_type_id).end())
                    rules_to_optimize->at(rewriting_type_id).push_back(rule);
                break;
            }
        }

        std::shared_ptr<AspifLiteral> predicate;

        if(is_an_auxiliar_predicate){

            auto element = aux_predicates_instances[rewriting_type_id].find(std::abs(predicate_id));

            if(element == aux_predicates_instances[rewriting_type_id].end()){
                predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
                aux_predicates_instances[rewriting_type_id].insert(std::pair<int, std::shared_ptr<AspifLiteral>>(std::abs(predicate_id), predicate));
            } else {
                predicate = element->second;
            }

            // It helps to save computation time later, during the Reversing Phase
            predicate->IncrementOccurrencesInBodies();

            if(optimize_reversing_activated && rule->AuxiliarPredicatesInHeadNumber() == 1){
                
                // A rule with an auxiliar predicate in head has a unique literal in head
                (*rule->GetAuxiliarPredicatesInHead().front()).AddLiteralDependency(predicate);
            }

        } else {
            predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
        }

        if(body_t == WeightBody){
            int weight;

            sstream >> weight;
            rule->AddInBody(predicate, weight, (predicate_id >= 0));
            
        } else {
            rule->AddInBody(predicate, (predicate_id >= 0));
        }
    }
    
}

// Stores a single Minimize Statement of the aspif format, which before was analyzed
// and that analysis proves that it contains auxiliar predicates

void AspifParser::StoreSingleMinimizeStatement(std::string input_line, std::string &full_input_line, const int &priority){
    
    std::istringstream sstream(input_line);
    std::shared_ptr<AspifMinimizeStatement> rule(new AspifMinimizeStatement());
    rule->SetPriority(priority);
    rule->SetEncodingLine(full_input_line);
    
    int literals_number;

    sstream >> literals_number;

    for (int i = 0; i < literals_number; i++)
    {
        int predicate_id;
        bool is_an_auxiliar_predicate = false;
        int rewriting_type_id = -1;

        sstream >> predicate_id;
        for (auto j = aux_predicates.begin(); j != aux_predicates.end(); j++)
        {
            if(std::abs(predicate_id) == j->first){
                is_an_auxiliar_predicate = true;
                rewriting_type_id = j->second;
                if(find(rules_to_optimize->at(rewriting_type_id).begin(), rules_to_optimize->at(rewriting_type_id).end(), rule) == rules_to_optimize->at(rewriting_type_id).end())
                    rules_to_optimize->at(rewriting_type_id).push_back(rule);
                break;
            }

        }

        int weight;

        sstream >> weight;

        std::shared_ptr<AspifLiteral> predicate;

        if(is_an_auxiliar_predicate){
            auto element = aux_predicates_instances[rewriting_type_id].find(std::abs(predicate_id));

            if(element == aux_predicates_instances[rewriting_type_id].end()){
                predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
                aux_predicates_instances[rewriting_type_id].insert(std::pair<int, std::shared_ptr<AspifLiteral>>(std::abs(predicate_id), predicate));
            } else {
                predicate = element->second;
            }

            // It helps to save computation time later, during the Reversing Phase
            predicate->IncrementOccurrencesInBodies();

        } else {
            predicate = std::make_shared<AspifLiteral>(std::abs(predicate_id), is_an_auxiliar_predicate);
        }

        rule->AddInBody(predicate, weight, (predicate_id >= 0));
    }
}