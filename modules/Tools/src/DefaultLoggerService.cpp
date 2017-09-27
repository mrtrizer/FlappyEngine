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

void DefaultLoggerService::log(const char* format, ...){
   va_list arglist;
   va_start(arglist, format);
   logVArg(format, arglist);
   va_end(arglist);
}

void DefaultLoggerService::logVArg(const char* format, va_list arglist) {
    std::vsnprintf(m_outputBuff.data(), m_outputBuff.size(), format, arglist );
    m_consoleManager->print(m_outputBuff.data());
}

}
