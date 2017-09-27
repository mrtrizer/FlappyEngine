#pragma once

#include "IConsoleService.h"

namespace flappy {

class DefaultConsoleService: public IConsoleService {
public:
    void print(const char *str) final;
};

} // flappy
