#include "../../include/main_components/central_data_manager.h"
using namespace aspsio;

CentralDataManager::CentralDataManager(int argc, char const *argv[]):input_manager(argc, argv), encoding_file_opened(false){
    aspif_parser = nullptr;
}

//  Main data flow of this ASP Solving Optimizing Tool

void CentralDataManager::StartReversing(){

    if(!input_manager.HelpRequired()){

        //  Opens the encoding file selected
        if(input_manager.OpenInputEncoding()){
            
            encoding_file_opened = true;

            //  Creates a parser for Aspif Format, passing to it all data structures
            //  needed to store rules that must be optimized

            aspif_parser = new AspifParser(input_manager.GetInputEncoding(), rules_to_optimize, patterns_to_match);
            
            int reversing_index = 0;
            int aggregate_index = -1;
            

            // Adds rewriting patterns that auxiliar predicates has to match

            if(input_manager.IsIsolatedVarsProjectionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_isolated");
                reversing_index++;
            }

            if(input_manager.IsAProjectionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_p_alpha");
                reversing_index++;
            }

            if(input_manager.IsBProjectionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_p_beta");
                reversing_index++;
            }

            if(input_manager.IsDecompositionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_decomp");
                reversing_index++;
            }

            if(input_manager.IsDecompositionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_p_decomp_isolated");
                reversing_index++;
            }

            if(input_manager.IsFunTermsProjectionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_function");
                reversing_index++;
            }

            if(input_manager.IsAggregatesProjectionReverseActivated()){
                aspif_parser->AddReverseParsingOption("aux_aggregate");
                aggregate_index = reversing_index;
                reversing_index++;
            }

            // Starts parsing phase
            aspif_parser->StartAnalysis();


            //  Performs Reversing for each Rewriting Method


            //  Creates a Reverser for each Rewriting Type
            for (int i = 0; i < reversing_index; i++)
            {
                bool limit_body = false;
                if(i == aggregate_index)
                    limit_body = true;
                
                reversers.push_back(AspifRewritingReverser(rules_to_optimize[i], aspif_parser->GetAuxPredicatesInstances()[i], input_manager.GetInputEncoding(), limit_body, rules_to_optimize));
                if(!input_manager.IsDuplicatesCheckingActivated())
                    reversers.back().DisableDuplicateChecking();
            }

            if(input_manager.OptimizeReversingActivated()){
                int predicates_removed;

                do {
                    predicates_removed = 0;

                    for (int i = 0; i < reversers.size(); i++)
                    {
                        predicates_removed += reversers[i].RemoveUselessPredicates(predicates_that_will_not_be_reversed);
                    }

                } while (predicates_removed > 0);
            }

            int reverses_done;

            do {
                reverses_done = 0;

                for (int i = 0; i < reversers.size(); i++)
                {
                    reverses_done += reversers[i].DoReverse();
                }

            } while (reverses_done > 0);

            //  Each rule is printed into the encoding data parsered

            for (int i = 0; i < rules_to_optimize.size(); i++)
            {
                for (auto it = rules_to_optimize[i].begin(); it !=  rules_to_optimize[i].end(); it++)
                {
                    for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
                    {
                            (*it2)->DoOutput();
                    }
                }
            }
        }
    }
}

std::string CentralDataManager::GetOutput(){
    if(input_manager.HelpRequired()){
        
        std::string help_output = "";

        help_output += "ASP Solving Input Optimizer - Help\n\n\n";
        help_output += "Introduction:\n";
        help_output += "--------------------\n";
        help_output += "ASP Solving Input Optimizer is a tool that makes optimizations to an input for an ASP Solver,\n";
        help_output += "like CLASP or WASP, before the solver reads this input file. It, now, supports an input format called \"aspif\",\n";
        help_output += "so it makes transformations to a file written using this format and creates a new, optimized, input encoded file.\n\n";
        help_output += "Options:\n";
        help_output += "--------------------\n";
        help_output += "--optimize-reversing\t\t\tAvoid to Reverse rules that need to be duplicated to be reversed";
        help_output += "--no-decomp-reverse\t\t\tDisactivates the Reversing of rules rewritten using Decomposition Rewriting\n\n";
        help_output += "--no-a-proj-reverse\t\t\tDisactivates the Reversing of rules rewritten using alpha-Projection Rewriting\n\n";
        help_output += "--no-b-proj-reverse\t\t\tDisactivates the Reversing of rules rewritten using beta-Prpjection Rewriting\n\n";
        help_output += "--no-aggregates-reverse\t\t\tDisactivates the Reversing of rules rewritten using Aggregates Rewriting\n\n";
        help_output += "--no-isolated-vars-reverse\t\t\tDisactivates the Reversing of rules rewritten using Isolated Variables Managing\n\n";
        help_output += "--no-functerms-reverse\t\t\tDisactivates the Reversing of rules rewritten using Projecion Rewriting of Functional Terms\n\n";
        help_output += "--no-duplicates-checking\t\t\tDisactivates the controls of rules duplication, saving performance when its sure that there aren't rules duplication in the Aspif Encoding Input\n\n";
        help_output += "--all\t\t\tActivates all Reversing types\n";
        return help_output;
    }

    if(!encoding_file_opened)
        return "Input file cannot be found\n";
    
    std::list<std::string> *input_encoding = &input_manager.GetInputEncoding();
    std::string output = "";
    for (auto it = input_encoding->begin(); it != input_encoding->end(); it++)
    {
        if(*it != "")
            output += *it + "\n";
    }

    return output;
}

CentralDataManager::~CentralDataManager(){

    if(aspif_parser != nullptr)
        delete aspif_parser;
}