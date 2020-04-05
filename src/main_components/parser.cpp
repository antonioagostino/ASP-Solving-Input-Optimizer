#include "../../include/main_components/parser.h"
using namespace aspsio;

Parser::Parser(const std::vector<std::string> &input_data, std::vector<std::list<Rule*>> &rules_sets, 
                    std::vector<std::string> &pattern_set)
{
    input_encoding = &input_data;
    rules_to_optimize = &rules_sets;
    patterns_to_match = &pattern_set;
}

void Parser::AddReverseParsingOption(const std::string &pattern){
    rules_to_optimize->push_back(std::list<Rule*>());
    patterns_to_match->push_back(pattern);
}