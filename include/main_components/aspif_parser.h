#ifndef ASPIF_PARSER_H
#define ASPIF_PARSER_H
#include "parser.h"
#include <iostream>

namespace aspsio {
    
    // A specific implementation of a Parser that interpretates
    // the "aspif" format
    
    class AspifParser : public Parser {
        
        private:
            void SaveAuxiliarPredicates();
            void SaveRulesToOptimize();
            void ParseRuleStatement(std::string input_line, std::string &full_input_line);
            void ParseMinimizeStatement(std::string input_line, std::string &full_input_line);
            void StoreSingleRuleStatement(std::string input_line, std::string &full_input_line, const int &head_type);
            void StoreSingleMinimizeStatement(std::string input_line, std::string &full_input_line, const int &priority);

        public:
            AspifParser(std::vector<std::string> &input_data, std::vector<std::list<Rule*>> &rules_sets, 
                    std::vector<std::string> &pattern_set);
            virtual void StartAnalysis() override;

    };
}

#endif