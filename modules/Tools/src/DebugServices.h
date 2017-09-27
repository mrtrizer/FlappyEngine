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

    void log(const char* format, ...);

    void setLogger(std::shared_ptr<ILoggerService> logManager) { m_logger = logManager; }
    void setConsole(std::shared_ptr<IConsoleService> consoleManager) { m_console = consoleManager; }

private:
    std::shared_ptr<ILoggerService> m_logger;
    std::shared_ptr<IConsoleService> m_console;
};

}

#ifndef RELEASE
#define LOG(...) DebugServices::instance().log(" [TRACE] " __VA_ARGS__)
#else
#define LOG(...) (void)
#endif
#define LOGI(...) DebugServices::instance().log("\x1b[32m [INFO] \x1b[0m" __VA_ARGS__)
#define LOGW(...) DebugServices::instance().log("\x1b[33m [WARNING] \x1b[0m" __VA_ARGS__)
#define LOGE(...) DebugServices::instance().log("\x1b[31m [ERROR] \x1b[0m" __VA_ARGS__)

#define VOID_VALUE void()
#define ERROR_MSG(value, ...) LOGE(__VA_ARGS__), LOGE("%s:%d\n", __FILE__, __LINE__), value;
