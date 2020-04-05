#include "../../include/main_components/aspif_parser.h"
using namespace aspsio;

AspifParser::AspifParser(const std::vector<std::string> &input_data, std::vector<std::list<Rule*>> &rules_sets, 
                    std::vector<std::string> &pattern_set):Parser(input_data, rules_sets, pattern_set)
{    
   
}

void AspifParser::StartAnalysis(){
    SaveAuxiliarPredicates();
    SaveRulesToOptimize();
}

// Controls "output statements" of the aspif format
// which usually are placed at the end of the encoded file.
// Then it saves them in a data structure, so they can be used later.

void AspifParser::SaveAuxiliarPredicates(){
    
    line_to_parse = input_encoding->size() - 2;
    bool show_section_ended = false;

    while (!show_section_ended)
    {
        int statement_type, name_lenght, predicate_id, condition_lenght;
        std::string predicate_name;

        std::istringstream sstream(input_encoding->at(line_to_parse));
        sstream >> statement_type >> name_lenght >> predicate_name >> condition_lenght >> predicate_id;
        if(statement_type == 4){
            for (int i = 0; i < patterns_to_match->size(); i++)
            {
                if(predicate_name.substr(0, patterns_to_match->at(i).size()) == patterns_to_match->at(i)){

                    // The first element is the predicate's id
                    // The second element is the type of Rewriting
                    aux_predicates.push_back(std::pair<int, int>(predicate_id, i));
                    break;
                }

            }

            line_to_parse--;
        } else {
            show_section_ended = true;
        }
        
    }
}

// Checks if a rule contains an auxiliar predicate, if it's true
// save an abstraction of this rule in a data structure

void AspifParser::SaveRulesToOptimize(){

    //We can avoid to parse the aspif header (so we start from index 1)
    for (int i = 1; i < line_to_parse; i++)
    {
        int statement_type = std::stoi(input_encoding->at(i).substr(0, 1));

        if(statement_type == 1){ //We are parsing a rule statement
            ParseRuleStatement(input_encoding->at(i).substr(2));
        } else if(statement_type == 2){ //We are parsing a minimize statement
            ParseMinimizeStatement(input_encoding->at(i).substr(2));
        }

    }
}

// It parses an Aspif Rule Statement to check if it contains auxiliar predicates
// if it's true, calls a method to store it

void AspifParser::ParseRuleStatement(const std::string &input_line){

    bool rule_must_be_optimized = false;
    int head_type;
    int head_size;
    std::istringstream sstream(input_line);

    sstream >> head_type >> head_size;

    for (int x = 0; x < head_size && !rule_must_be_optimized; x++)
    {
        int predicate_id;
        sstream >> predicate_id;
        for (int j = 0; j < aux_predicates.size(); j++)
        {
            if(predicate_id == aux_predicates[j].first){
                rule_must_be_optimized = true;
                break;
            }
        }
        
    }
    
    if(rule_must_be_optimized){
        StoreSingleRuleStatement(input_line.substr(2), head_type);
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
            for (int j = 0; j < aux_predicates.size(); j++)
            {
                if(predicate_id == aux_predicates[j].first){
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
            StoreSingleRuleStatement(input_line.substr(2), head_type);
        
    }
}

// It parses an Aspif Minimize Statement to check if it contains auxiliar predicates
// if it's true, calls a method to store it

void AspifParser::ParseMinimizeStatement(const std::string &input_line){

    bool rule_must_be_optimized = false;
    std::istringstream sstream(input_line);
    int priority, literals_number;

    sstream >> priority;
    sstream >> literals_number;

    for (int i = 0; i < literals_number && !rule_must_be_optimized; i++)
    {
        int predicate_id;

        sstream >> predicate_id;
        for (int j = 0; j < aux_predicates.size(); j++)
        {
            if(predicate_id == aux_predicates[j].first){
                rule_must_be_optimized = true;
                break;
            }
        }
    }

    if(rule_must_be_optimized)
        StoreSingleMinimizeStatement(input_line.substr(2), priority);
    

}

// Stores a single Rule Statement of the aspif format, which before was analyzed
// and that analysis proves that it contains auxiliar predicates

void AspifParser::StoreSingleRuleStatement(const std::string &input_line, const int &head_type){
    
    std::istringstream sstream(input_line);
    AspifRuleStatement *rule = new AspifRuleStatement();
    HeadType head_t;

    if(head_type == 0)
        head_t = Disjunction;
    else if(head_type == 1)
        head_t = Choice;

    
    rule->SetHeadType(head_t);
    int head_size;
    bool head_aux_found = false;

    sstream >> head_size;

    for (int i = 0; i < head_size; i++)
    {
        int predicate_id;
        bool is_an_auxiliar_predicate = false;

        sstream >> predicate_id;
        if(!head_aux_found){

            for (int j = 0; j < aux_predicates.size(); j++)
            {
                if(predicate_id == aux_predicates[j].first){
                    is_an_auxiliar_predicate = true;
                    head_aux_found = true;
                    rules_to_optimize->at(aux_predicates[j].second).push_back(rule);
                    break;
                }
            }

        }

        rule->AddInHead(predicate_id, is_an_auxiliar_predicate);

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

        sstream >> predicate_id;
        for (int j = 0; j < aux_predicates.size(); j++)
        {
            if(predicate_id == aux_predicates[j].first){
                is_an_auxiliar_predicate = true;
                rules_to_optimize->at(aux_predicates[j].second).push_back(rule);
                break;
            }
        }

        rule->AddInBody(predicate_id, is_an_auxiliar_predicate);

        if(body_t == WeightBody){
            int weight;

            sstream >> weight;
            rule->AddWeight(weight);
        }

    }
    
}

// Stores a single Minimize Statement of the aspif format, which before was analyzed
// and that analysis proves that it contains auxiliar predicates

void AspifParser::StoreSingleMinimizeStatement(const std::string &input_line, const int &priority){
    
    std::istringstream sstream(input_line);
    AspifMinimizeStatement *rule = new AspifMinimizeStatement();
    rule->SetPriority(priority);
    
    int literals_number;

    sstream >> literals_number;

    for (int i = 0; i < literals_number; i++)
    {
        int predicate_id;
        bool is_an_auxiliar_predicate = false;

        sstream >> predicate_id;
        for (int j = 0; j < aux_predicates.size(); j++)
        {
            if(predicate_id == aux_predicates[j].first){
                is_an_auxiliar_predicate = true;
                rules_to_optimize->at(aux_predicates[j].second).push_back(rule);
                break;
            }

        }

        rule->AddInBody(predicate_id, is_an_auxiliar_predicate);

        int weight;

        sstream >> weight;

        rule->AddWeight(weight);
    }
}