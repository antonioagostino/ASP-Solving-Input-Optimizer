#ifndef CENTRAL_DATA_MANAGER_H
#define CENTRAL_DATA_MANAGER_H
#include "input_manager.h"
#include "aspif_parser.h"
#include "aspif_rewriting_reverser.h"

namespace aspsio {
    
    //  Contains all components that compute tasks for optimizing the input
    //  for an ASP Solver, and controls the execution flow

    class CentralDataManager {

        private:
            InputManager input_manager;
            AspifParser *aspif_parser;

            //  Each cell of this Vector represents a rewriting category
            //  so rules which need to be reversed are divided in categories
            //  and organized by Auxiliar Predicates appearences
            std::vector<std::unordered_map<int, std::list<std::shared_ptr<AspifStatement>>>> rules_to_optimize;

            //  Each rewriting method uses a standard name for its auxiliar
            //  predicates, so these patterns are used to find them
            std::vector<std::string> patterns_to_match;

            std::vector<AspifRewritingReverser> reversers;

            //  Predicates that will not be reversed because they need
            //  rules duplication
            std::unordered_map<int, int> predicates_that_will_not_be_reversed;

            bool encoding_file_opened;
            
        public:
            CentralDataManager(int argc, char const *argv[]);
            void StartReversing();
            std::string GetOutput();
            ~CentralDataManager();
    };
}

#endif