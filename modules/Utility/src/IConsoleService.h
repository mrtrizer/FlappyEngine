#pragma once

#include <string>

#include "LogMessageType.h"

namespace flappy {

class IConsoleService {
public:
    virtual ~IConsoleService() = default;
    virtual void print(LogMessageType, const char *) = 0;
};

} // flappy
