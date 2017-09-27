#include "DefaultConsoleService.h"

#include <iostream>

namespace flappy {

void DefaultConsoleService::print(const char* str) {
    std::cout << str << std::endl;
}

} // flappy
