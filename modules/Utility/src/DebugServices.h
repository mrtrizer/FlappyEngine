#pragma once

#include "ILoggerService.h"
#include "IConsoleService.h"

#include <cstdarg>
#include <memory>

namespace flappy {

class DebugServices {
public:

    DebugServices();
    virtual ~DebugServices() = default;
    DebugServices(const DebugServices&) = delete;
    DebugServices& operator=(const DebugServices&) = delete;
    DebugServices(DebugServices&&) = delete;
    DebugServices& operator=(DebugServices&&) & = delete;

    static DebugServices& instance() {
        static DebugServices debug;
        return debug;
    }

    std::shared_ptr<ILoggerService> logger() { return m_logger; }
    void setLogger(std::shared_ptr<ILoggerService> logManager) { m_logger = logManager; }

    std::shared_ptr<IConsoleService> console() { return m_console; }
    void setConsole(std::shared_ptr<IConsoleService> consoleManager) { m_console = consoleManager; }

private:
    std::shared_ptr<IConsoleService> m_console;
    std::shared_ptr<ILoggerService> m_logger;
};

}

#ifndef RELEASE
#define LOG(...) DebugServices::instance().logger()->log(LogMessageType::TRACE, __VA_ARGS__)
#else
#define LOG(...) (void)
#endif
#define LOGI(...) DebugServices::instance().logger()->log(LogMessageType::INFO, __VA_ARGS__)
#define LOGW(...) DebugServices::instance().logger()->log(LogMessageType::WARNING, __VA_ARGS__)
#define LOGE(...) DebugServices::instance().logger()->log(LogMessageType::ERROR, __VA_ARGS__)

#define VOID_VALUE void()
#define ERROR_MSG(value, ...) LOGE(__VA_ARGS__), LOGE("%s:%d\n", __FILE__, __LINE__), value;
