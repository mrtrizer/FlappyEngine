#pragma once

#include "IConsoleService.h"

namespace flappy {

class DefaultConsoleService: public IConsoleService {
public:
    void print(LogMessageType messageType, const char *str) final;
};

} // flappy
