#pragma once

#include <ILogManager.h>
#include <IConsoleManager.h>

#include <cstdarg>
#include <memory>

namespace flappy {

class Debug {
public:

    Debug();
    virtual ~Debug() = default;
    Debug(const Debug&) = delete;
    Debug& operator=(const Debug&) = delete;
    Debug(Debug&&) = delete;
    Debug& operator=(Debug&&) & = delete;

    static Debug& instance() {
        static Debug debug;
        return debug;
    }

    void log(const char* format, ...);

    void setLogger(std::shared_ptr<ILogManager> logManager) { m_logger = logManager; }
    void setConsole(std::shared_ptr<IConsoleManager> consoleManager) { m_console = consoleManager; }

private:
    std::shared_ptr<ILogManager> m_logger;
    std::shared_ptr<IConsoleManager> m_console;
};

}

#ifndef RELEASE
#define LOG(...) Debug::instance().log("[TRACE]" __VA_ARGS__)
#else
#define LOG(...) (void)
#endif
#define LOGI(...) Debug::instance().log("\x1b[32m [INFO] \x1b[0m" __VA_ARGS__)
#define LOGW(...) Debug::instance().log("\x1b[33m [WARNING] \x1b[0m" __VA_ARGS__)
#define LOGE(...) Debug::instance().log("\x1b[31m [ERROR] \x1b[0m" __VA_ARGS__)

#define VOID_VALUE void()
#define ERROR_MSG(value, ...) LOGE(__VA_ARGS__), LOGE("%s:%d\n", __FILE__, __LINE__), value;
