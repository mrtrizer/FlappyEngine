#pragma once

#include <string>
#include <memory>

#include "ILogManager.h"

namespace flappy {

class IConsoleManager;

class DefaultLogManager : public ILogManager {
public:
    DefaultLogManager(std::shared_ptr<flappy::IConsoleManager> consoleManager);

    void log(const char* format, ...) final;
    void logVArg(const char* format, va_list arglist) final;

private:
    std::string m_outputBuff;
    std::shared_ptr<flappy::IConsoleManager> m_consoleManager;
};

} // flappy
