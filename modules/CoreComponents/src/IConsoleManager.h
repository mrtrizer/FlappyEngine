#pragma once

#include <Manager.h>

namespace flappy {

class IConsoleManager : public Manager<IConsoleManager> {
public:
    virtual void print(std::string str) = 0;
};

} // flappy
