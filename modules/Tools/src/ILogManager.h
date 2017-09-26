#pragma once

#include <cstdarg>

namespace flappy {

class ILogManager {
public:
    virtual void log(const char* format, ...) = 0;
    virtual void logVArg(const char* format, va_list arglist) = 0;
};

} // flappy
