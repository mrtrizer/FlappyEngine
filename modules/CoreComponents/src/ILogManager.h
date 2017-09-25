#pragma once

#include <Manager.h>

namespace flappy {

class ILogManager : public Manager<ILogManager> {
public:
    virtual void log(const char* format, ...) = 0;
};

} // flappy
