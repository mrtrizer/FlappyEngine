#include "DefaultLogManager.h"

#include <cstdio>

#include "IConsoleManager.h"

namespace flappy {

DefaultLogManager::DefaultLogManager()
    : m_outputBuff(1024, ' ')
{
    addDependency(IConsoleManager::id());
}

void DefaultLogManager::log(const char* format, ...){
   va_list arglist;
   va_start(arglist, format);
   std::vsnprintf(&m_outputBuff.front(), m_outputBuff.size(), format, arglist );
   manager<IConsoleManager>()->print(m_outputBuff);
   va_end(arglist);
}

}
