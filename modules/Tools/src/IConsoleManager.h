#pragma once

#include <string>

namespace flappy {

class IConsoleManager{
public:
    virtual void print(std::string str) = 0;
};

} // flappy
