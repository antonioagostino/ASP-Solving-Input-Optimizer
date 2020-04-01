#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
namespace aspsio {

    // A generalization of a Parser, it works as interface for 
    // all types of parser
    
    class Parser {
        protected:
            std::vector<std::string> input_encoding; 
        public:
            Parser(const std::vector<std::string> &input_data);
            virtual void StartAnalysis() = 0;
    };
}

#endif