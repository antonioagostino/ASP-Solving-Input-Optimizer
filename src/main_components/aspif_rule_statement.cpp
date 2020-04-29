#include "../../include/main_components/aspif_rule_statement.h"
using namespace aspsio;

AspifRuleStatement::AspifRuleStatement():head_type(Disjunction), lower_bound(-1){}

// Print the new rule structure into the input encoding data

void AspifRuleStatement::DoOutput(){

    if(!useless){
        std::string newline = "1 ";
        
        newline += std::to_string(head_type) + " " + std::to_string(head.size()) + " ";

        for (auto it = head.begin(); it != head.end(); it++)
        {
            newline += std::to_string((*it)->GetId()) + " ";
        }
        
        newline += std::to_string(body_type) + " ";

        if(body_type == WeightBody)
            newline += std::to_string(lower_bound) + " ";

        newline += std::to_string(body.size()) + " ";

        for (auto it = body.begin(); it != body.end(); it++)
        {
            newline += std::to_string(it->first->GetId()) + " ";
            if(body_type == WeightBody)
                newline += std::to_string(*(it->second)) + " ";
        }

        *encoding_line = newline;
    } else {
        *encoding_line = "";
    }
}

std::shared_ptr<AspifStatement> AspifRuleStatement::Clone(){
    std::shared_ptr<AspifRuleStatement> new_rule(new AspifRuleStatement());
    new_rule->SetBodyType(body_type);
    new_rule->SetHeadType(head_type);
    new_rule->SetLowerBound(lower_bound);

    for (auto it = head.begin(); it != head.end(); it++)
    {
        new_rule->AddInHead(*it);
    }

    for (auto it = body.begin(); it != body.end(); it++)
    {
        new_rule->AddInBody(it->first, it->second);
    }

    return new_rule;
}