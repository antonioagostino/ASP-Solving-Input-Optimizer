#include "../../include/main_components/aspif_rule_literal.h"
using namespace aspsio;

AspifRuleLiteral::AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal, const int &_weight, 
const bool &_positive):literal(_literal), weight(_weight), positive_literal(_positive), has_weight(true){
    
}

AspifRuleLiteral::AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal, const bool &_positive):literal(_literal), 
weight(0), has_weight(false), positive_literal(_positive){
    
}

AspifRuleLiteral::AspifRuleLiteral(std::shared_ptr<AspifLiteral> _literal):literal(_literal), 
weight(0), has_weight(false), positive_literal(true){
    
}