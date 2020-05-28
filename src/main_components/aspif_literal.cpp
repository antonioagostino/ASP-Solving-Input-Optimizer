#include "../../include/main_components/aspif_literal.h"
using namespace aspsio;

AspifLiteral::AspifLiteral(const int &_id, const bool &_auxiliar):id(_id), auxiliar(_auxiliar), 
occurrences_in_bodies(0), occurrences_in_heads(0)
{
    
}

void AspifLiteral::AddLiteralDependency(std::shared_ptr<AspifLiteral> literal){
    if(std::find(depends_on.begin(), depends_on.end(), literal) == depends_on.end())
        depends_on.push_back(literal);
}