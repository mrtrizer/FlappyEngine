#include "DefaultLoggerService.h"

#include <cstdio>
#include <cstdarg>

#include "IConsoleService.h"

namespace flappy {

DefaultLoggerService::DefaultLoggerService(std::shared_ptr<IConsoleService> consoleManager)
    : m_outputBuff(1024)
    , m_consoleManager(consoleManager)
{
}

void DefaultLoggerService::log(LogMessageType messageType, const char* format, ...){
   va_list arglist;
   va_start(arglist, format);
   logVArg(messageType, format, arglist);
   va_end(arglist);
}

void DefaultLoggerService::logVArg(LogMessageType messageType, const char* format, va_list arglist) {
    std::vsnprintf(m_outputBuff.data(), m_outputBuff.size(), format, arglist );
    m_consoleManager->print(messageType, m_outputBuff.data());
}

}
