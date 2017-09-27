#pragma once

#include <vector>
#include <memory>

#include "ILoggerService.h"

namespace flappy {

class IConsoleService;

class DefaultLoggerService : public ILoggerService {
public:
    DefaultLoggerService(std::shared_ptr<flappy::IConsoleService> consoleManager);

    void log(LogMessageType, const char* format, ...) final;
    void logVArg(LogMessageType, const char* format, va_list arglist) final;

private:
    std::vector<char> m_outputBuff;
    std::shared_ptr<flappy::IConsoleService> m_consoleManager;
};

} // flappy
