#pragma once

#include <IConsoleManager.h>

namespace flappy {

class StdConsoleManager: public IConsoleManager {
public:
    void print(const char *str) final;
};

} // flappy
