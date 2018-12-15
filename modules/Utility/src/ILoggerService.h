#pragma once

#include <cstdarg>

#include "LogMessageType.h"

namespace flappy {

class ILoggerService {
public:
    virtual void log(LogMessageType, const char* format, ...) = 0;
    virtual void logVArg(LogMessageType, const char* format, va_list arglist) = 0;
};

} // flappy
