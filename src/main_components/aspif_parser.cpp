#include "../../include/main_components/aspif_parser.h"
using namespace aspsio;

AspifParser::AspifParser(const std::vector<std::string> &input_data):Parser(input_data){
    
    //A print to check if it works
    std::cout << Parser::input_encoding[0] << std::endl;
}

void AspifParser::StartAnalysis(){
    //...
}