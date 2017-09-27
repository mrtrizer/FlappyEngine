#include "DebugServices.h"

#include "DefaultLoggerService.h"
#include "DefaultConsoleService.h"

namespace flappy {

DebugServices::DebugServices() {
    m_console = std::make_shared<DefaultConsoleService>();
    m_logger = std::make_shared<DefaultLoggerService>(m_console);
}

void DebugServices::log(const char* format, ...) {
    if (m_logger != nullptr) {
        va_list arglist;
        va_start(arglist, format);
        m_logger->logVArg(format, arglist);
        va_end(arglist);
    }
}

} // flappy
