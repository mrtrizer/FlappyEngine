#pragma once

#include <string>

namespace flappy {

class IConsoleManager{
public:
    virtual void print(const char *str) = 0;
};

} // flappy
