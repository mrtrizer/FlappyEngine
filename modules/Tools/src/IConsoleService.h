#pragma once

#include <string>

namespace flappy {

class IConsoleService{
public:
    virtual void print(const char *str) = 0;
};

} // flappy
