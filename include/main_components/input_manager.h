#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <iostream>
#include <locale>
#include <fstream>
#include <vector>

namespace aspsio {

    // Reads the input parameters and runs the tasks to be performed.
    // Tells which Reverse Component must be activated
    // and reads the input file, creating a buffer with its content
    
    class InputManager {
        private:
            std::vector<std::string> input_data;
            std::string *input_parameters;
            std::string input_file_path;
            std::ifstream input_file;
            bool decomposition_reverse_activated;
            bool a_projection_reverse_activated;
            bool b_projection_reverse_activated;
            bool fun_terms_projection_reverse_activated;
            bool isolated_vars_projection_reverse_activated;
            bool help_required;
            int params_number;


            void ManageInput(const std::string &input_argument);
            bool IsExecutionOption(const std::string &execution_option);
            void ProcessExecutionOption(const std::string &execution_option);

        public:
            InputManager(int argc, char const *argv[]);
            bool HelpRequired(){ return help_required; }
            std::vector<std::string>& OpenInputEncoding();
            bool IsDecompositionReverseActivated(){ return decomposition_reverse_activated; }
            bool IsAProjectionReverseActivated(){ return a_projection_reverse_activated; }
            bool IsBProjectionReverseActivated(){ return b_projection_reverse_activated; }
            bool IsFunTermsProjectionReverseActivated(){ return fun_terms_projection_reverse_activated; }
            bool IsIsolatedVarsProjectionReverseActivated(){ return isolated_vars_projection_reverse_activated; }
            bool InputFilePathSelected(){ return !(input_file_path == ""); }
            ~InputManager();
    };

}
#endif