#include "../../include/main_components/central_data_manager.h"
using namespace aspsio;

CentralDataManager::CentralDataManager(int argc, char const *argv[]):input_manager(argc, argv){
    if(!input_manager.HelpRequired()){
        if(input_manager.InputFilePathSelected()){
            aspif_parser = new AspifParser(input_manager.OpenInputEncoding(), rules_to_optimize, patterns_to_match);
            aspif_parser->AddReverseParsingOption("aux_isolated");
            aspif_parser->StartAnalysis();

            // A print to check if it works
            std::cout << rules_to_optimize.size() << " " << rules_to_optimize[0].size() << std::endl;
        } else {
            std::cout << "No input file selected";
        }
    }
}