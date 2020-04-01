#include "../../include/main_components/central_data_manager.h"
using namespace aspsio;

CentralDataManager::CentralDataManager(int argc, char const *argv[]):input_manager(argc, argv){
    if(!input_manager.HelpRequired()){
        aspif_parser = new AspifParser(input_manager.OpenInputEncoding());
    }
}