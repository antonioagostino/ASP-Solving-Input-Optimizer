#include "../../include/main_components/aspif_minimize_statement.h"
using namespace aspsio;

AspifMinimizeStatement::AspifMinimizeStatement():priority(0){}

// Print the new rule structure into the input encoding data

void AspifMinimizeStatement::DoOutput(){
    if(!useless){
        std::string newline = "2 ";
        
        newline += std::to_string(priority) + " " + std::to_string(body.size()) + " ";

        for (auto it = body.begin(); it != body.end(); it++)
        {
            int multipl;
            if(it->IsPositive())
                multipl = 1;
            else
                multipl = -1;
            
            newline += std::to_string(it->GetLiteral()->GetId() * multipl) + " " + std::to_string(it->GetWeight()) + " ";
        }

        *encoding_line = newline;
    } else {
        *encoding_line = "";
    }
}

std::shared_ptr<AspifStatement> AspifMinimizeStatement::Clone(){
    std::shared_ptr<AspifMinimizeStatement> new_rule(new AspifMinimizeStatement());
    new_rule->SetPriority(priority);

    for (auto it = body.begin(); it != body.end(); it++)
    {
        new_rule->AddInBody(it->GetLiteral(), it->GetWeight(), it->IsPositive());
    }

    return new_rule;
    
}