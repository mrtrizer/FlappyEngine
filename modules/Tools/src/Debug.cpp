#include "Debug.h"

#include "DefaultLogManager.h"
#include "StdConsoleManager.h"

namespace flappy {

Debug::Debug() {
    m_console = std::make_shared<StdConsoleManager>();
    m_logger = std::make_shared<DefaultLogManager>(m_console);
}

void Debug::log(const char* format, ...) {
    if (m_logger != nullptr) {
        va_list arglist;
        va_start(arglist, format);
        m_logger->logVArg(format, arglist);
        va_end(arglist);
    }
}

} // flappy
