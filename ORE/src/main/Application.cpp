#include "../ORE.h"

int main()
{
    ORE::Log::Init();
    ORE_CORE_TRACE("Project Initialization!! - TRACE");
    ORE_CORE_INFO("Project Initialization!! - INFO");
    ORE_CORE_WARN("Project Initialization!! - WARN");
    ORE_CORE_ERROR("Project Initialization!! - ERROR");
    ORE_CORE_CRITICAL("Project Initialization!! - CRITICAL");

    std::cout << "Project Initialization!!";
    std::cin.get();
}