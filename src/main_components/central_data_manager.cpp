#include "../../include/main_components/central_data_manager.h"
using namespace aspsio;

CentralDataManager::CentralDataManager(int argc, char const *argv[]):input_manager(argc, argv), encoding_file_opened(false){
    aspif_parser = nullptr;
}

//  Main data flow of this ASP Solving Optimizing Tool

void CentralDataManager::StartReversing(){

    if(!input_manager.HelpRequired()){
        if(input_manager.InputFilePathSelected()){
            
            //  Opens the encoding file selected
            if(input_manager.OpenInputEncoding()){

                encoding_file_opened = true;

                //  Performs rules parsing
                aspif_parser = new AspifParser(input_manager.GetInputEncoding(), rules_to_optimize, patterns_to_match);
                if(input_manager.IsIsolatedVarsProjectionReverseActivated())
                    aspif_parser->AddReverseParsingOption("aux_isolated");

                if(input_manager.IsAProjectionReverseActivated())
                    aspif_parser->AddReverseParsingOption("aux_a_proj");
                
                if(input_manager.IsBProjectionReverseActivated())
                    aspif_parser->AddReverseParsingOption("aux_b_proj");

                if(input_manager.IsDecompositionReverseActivated())
                    aspif_parser->AddReverseParsingOption("aux_decomp");
                
                if(input_manager.IsFunTermsProjectionReverseActivated())
                    aspif_parser->AddReverseParsingOption("aux_func_term");
                    
                aspif_parser->StartAnalysis();

                //  Performs Reversing for each Rewriting Method
                int reversing_index = 0;

                if(input_manager.IsIsolatedVarsProjectionReverseActivated()){
                    RewritingReverser isolated_reverser(rules_to_optimize[reversing_index], aspif_parser->GetAuxPredicatesInstances());
                    reversing_index++;
                    isolated_reverser.DoReverse();
                }

                if(input_manager.IsAProjectionReverseActivated()){
                    RewritingReverser a_proj_reverser(rules_to_optimize[reversing_index], aspif_parser->GetAuxPredicatesInstances());
                    reversing_index++;
                    a_proj_reverser.DoReverse();
                }

                if(input_manager.IsBProjectionReverseActivated()){
                    RewritingReverser b_proj_reverser(rules_to_optimize[reversing_index], aspif_parser->GetAuxPredicatesInstances());
                    reversing_index++;
                    b_proj_reverser.DoReverse();
                }

                if(input_manager.IsDecompositionReverseActivated()){
                    RewritingReverser decomp_reverser(rules_to_optimize[reversing_index], aspif_parser->GetAuxPredicatesInstances());
                    reversing_index++;
                    decomp_reverser.DoReverse();
                }

                if(input_manager.IsFunTermsProjectionReverseActivated()){
                    RewritingReverser fun_terms_reverser(rules_to_optimize[reversing_index], aspif_parser->GetAuxPredicatesInstances());
                    reversing_index++;
                    fun_terms_reverser.DoReverse();
                }

                //  Each rule is printed into the encoding data parsered

                for (int i = 0; i < rules_to_optimize.size(); i++)
                {
                    for (auto it = rules_to_optimize[i].begin(); it !=  rules_to_optimize[i].end(); it++)
                    {
                        (*it)->DoOutput();
                        (*it)->SetDeepDelete(true);
                    }
                    
                }
            }
        }
    }
}

std::string CentralDataManager::GetOutput(){
    if(input_manager.HelpRequired())
        //  NEED TO IMPLEMENT!
        return "Help Required\n";

    if(!input_manager.InputFilePathSelected())
        return "No input file selected\n";

    if(!encoding_file_opened)
        return "Input file cannot be found\n";
    
    std::vector<std::string> *input_encoding = &input_manager.GetInputEncoding();
    std::string output = "";
    for (int i = 0; i < input_encoding->size(); i++)
    {
        if(input_encoding->at(i) != "")
            output += input_encoding->at(i) + "\n";
    }

    return output;
}

CentralDataManager::~CentralDataManager(){

    if(aspif_parser != nullptr)
        delete aspif_parser;
    
    for (int i = 0; i < rules_to_optimize.size(); i++)
    {
        for (auto it = rules_to_optimize[i].begin(); it !=  rules_to_optimize[i].end(); it++)
        {
            delete (*it);
        }
        
    }
}