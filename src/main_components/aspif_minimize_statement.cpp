#include "../../include/main_components/aspif_minimize_statement.h"
using namespace aspsio;

AspifMinimizeStatement::AspifMinimizeStatement():priority(0){}

// Print the new rule structure into the input encoding data

void AspifMinimizeStatement::DoOutput(){
    std::string newline = "2 ";
    
    newline += std::to_string(priority) + " " + std::to_string(body.size()) + " ";

    for (auto it = body.begin(); it != body.end(); it++)
    {
        newline += (*it)->DoOutput();
    }

    *encoding_line = newline;
}