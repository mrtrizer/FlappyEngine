#include "DefaultLogManager.h"

#include <cstdio>
#include <cstdarg>

#include "IConsoleManager.h"

namespace flappy {

DefaultLogManager::DefaultLogManager(std::shared_ptr<IConsoleManager> consoleManager)
    : m_outputBuff(1024, ' ')
    , m_consoleManager(consoleManager)
{
}

void DefaultLogManager::log(const char* format, ...){
   va_list arglist;
   va_start(arglist, format);
   logVArg(format, arglist);
   va_end(arglist);
}

void DefaultLogManager::logVArg(const char* format, va_list arglist) {
    std::vsnprintf(&m_outputBuff.front(), m_outputBuff.size(), format, arglist );
    m_consoleManager->print(m_outputBuff);
}

}
