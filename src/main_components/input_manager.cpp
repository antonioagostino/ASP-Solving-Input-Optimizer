#include "../../include/main_components/input_manager.h"
using namespace aspsio;

InputManager::InputManager(int argc, char const *argv[]):help_required(false), input_file(""), 
a_projection_reverse_activated(true), b_projection_reverse_activated(true), fun_terms_projection_reverse_activated(true),
decomposition_reverse_activated(true), isolated_vars_projection_reverse_activated(true), 
aggregates_proj_reverse_activated(true), duplicates_checking(true), optimize_reversing_activated(false),
params_number(argc - 1)
{

    input_parameters = new std::string[params_number];

    for (int i = 0; i < params_number; i++)
    {
        if(help_required)
            break;
        
        input_parameters[i] = std::string(argv[i + 1]);
        ManageInput(input_parameters[i]);
    }
    
}

// Coordinates the input processing
void InputManager::ManageInput(const std::string &input_argument){

    if(IsExecutionOption(input_argument)){
        ProcessExecutionOption(input_argument);
    } else if(input_file_path == ""){
        input_file_path = input_argument;
    }
    
}


// Checks if an input parameter is valid, if not discards it
bool InputManager::IsExecutionOption(const std::string &execution_option){

    if(execution_option.size() < 2)
        return false;

    if(execution_option[0] == '-' && execution_option[1] == '-' && execution_option.size() >= 4)
        return true;

    std::locale loc;

    if(execution_option[0] == '-' && execution_option.size() == 2 && std::isalpha(execution_option[1], loc))
        return true;

    return false;
}

//Does operations depending on execution options obtained by input
void InputManager::ProcessExecutionOption(const std::string &execution_option){
    
    if(execution_option == "--help"){
        help_required = true;
    } else if(execution_option == "--all"){
        decomposition_reverse_activated = true;
        a_projection_reverse_activated = true;
        b_projection_reverse_activated = true;
        isolated_vars_projection_reverse_activated = true;
        fun_terms_projection_reverse_activated = true;
    } else if(execution_option == "--no-decomp-reverse"){
        decomposition_reverse_activated = false;
    } else if(execution_option == "--no-a-proj-reverse"){
        a_projection_reverse_activated = false;
    } else if(execution_option == "--no-b-proj-reverse"){
        b_projection_reverse_activated = false;
    } else if(execution_option == "--no-isolated-vars-reverse"){
        isolated_vars_projection_reverse_activated = false;
    } else if(execution_option == "--no-functerms-reverse"){
        fun_terms_projection_reverse_activated = false;
    } else if(execution_option == "--no-aggregates-reverse"){
        aggregates_proj_reverse_activated = false;
    } else if(execution_option == "--no-duplicates-checking"){
        duplicates_checking = false;
    } else if(execution_option == "--optimize-reversing"){
        optimize_reversing_activated = true;
    }
}

// Checks if input file exists and gets its content

bool InputManager::OpenInputEncoding(){
    
    std::string line;

    if(InputFilePathSelected()){
        input_file.open(input_file_path);
        if(input_file.good()){
            while (std::getline(input_file, line))
            {
                input_data.push_back(line);
            }
        }

        input_file.close();
        if(input_data.size() > 0)
            return true;

        return false;

    } else {

        while (std::getline(std::cin, line))
        {
            input_data.push_back(line);
        }

        if(input_data.size() > 0)
            return true;

        return false;
    }
}

InputManager::~InputManager(){
    delete[] input_parameters;
}