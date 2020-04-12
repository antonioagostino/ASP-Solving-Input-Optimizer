#include "../../include/main_components/central_data_manager.h"
using namespace aspsio;

int main(int argc, char const *argv[])
{
    CentralDataManager central_data_manager(argc, argv);
    central_data_manager.StartReversing();
    std::cout << central_data_manager.GetOutput();
    return 0;
}
