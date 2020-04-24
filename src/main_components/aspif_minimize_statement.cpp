#include "../../include/main_components/aspif_minimize_statement.h"
using namespace aspsio;

AspifMinimizeStatement::AspifMinimizeStatement():priority(0){}

// Print the new rule structure into the input encoding data

void AspifMinimizeStatement::DoOutput(){
    std::string newline = "2 ";
    
    newline += std::to_string(priority) + " " + std::to_string(body.size()) + " ";

    for (auto it = body.begin(); it != body.end(); it++)
    {
        newline += std::to_string(it->first->GetId()) + " " + std::to_string(*(it->second)) + " ";
    }

    *encoding_line = newline;
}

AspifStatement* AspifMinimizeStatement::Clone(){
    AspifMinimizeStatement *new_rule = new AspifMinimizeStatement();
    new_rule->SetPriority(priority);

    for (auto it = body.begin(); it != body.end(); it++)
    {
        new_rule->AddInBody(it->first, it->second);
    }

    return new_rule;
    
}