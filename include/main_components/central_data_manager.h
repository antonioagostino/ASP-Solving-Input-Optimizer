#ifndef CENTRAL_DATA_MANAGER_H
#define CENTRAL_DATA_MANAGER_H
#include "input_manager.h"
#include "aspif_parser.h"

namespace aspsio {
    
    // Contains all components that compute tasks for optimizing the input
    // for an ASP Solver, and controls the execution flow

    class CentralDataManager {

        private:
            InputManager input_manager;
            Parser *aspif_parser;
        public:
            CentralDataManager(int argc, char const *argv[]);
    };

}

#endif