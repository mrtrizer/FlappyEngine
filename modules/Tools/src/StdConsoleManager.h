#pragma once

#include <IConsoleManager.h>

namespace flappy {

class StdConsoleManager: public IConsoleManager {
public:
    void print(std::string str) final;
};

} // flappy
