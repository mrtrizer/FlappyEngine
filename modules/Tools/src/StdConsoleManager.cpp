#include "StdConsoleManager.h"

#include <iostream>

namespace flappy {

void StdConsoleManager::print(const char* str) {
    std::cout << str << std::endl;
}

} // flappy
