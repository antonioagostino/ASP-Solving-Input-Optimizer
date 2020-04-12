#include "../../include/main_components/aspif_rule_statement.h"
using namespace aspsio;

AspifRuleStatement::AspifRuleStatement():head_type(Disjunction), lower_bound(-1){}

// Print the new rule structure into the input encoding data

void AspifRuleStatement::DoOutput(){
    std::string newline = "1 ";
    
    newline += std::to_string(head_type) + " " + std::to_string(head.size()) + " ";

    for (auto it = head.begin(); it != head.end(); it++)
    {
        newline += (*it)->DoOutput();
    }
    
    newline += std::to_string(body_type) + " ";

    if(body_type == WeightBody)
        newline += std::to_string(lower_bound) + " ";

    newline += std::to_string(body.size()) + " ";

    for (auto it = body.begin(); it != body.end(); it++)
    {
        newline += (*it)->DoOutput();
    }

    *encoding_line = newline;
}