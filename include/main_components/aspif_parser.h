#ifndef ASPIF_PARSER_H
#define ASPIF_PARSER_H
#include "parser.h"
#include <iostream>

namespace aspsio {
    
    // A specific implementation of a Parser that interpretates
    // the "aspif" format
    
    class AspifParser : public Parser {
        
        public:
            AspifParser(const std::vector<std::string> &input_data);
            virtual void StartAnalysis() override;

    };
}

#endif