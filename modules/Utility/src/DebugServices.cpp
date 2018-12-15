#include "DebugServices.h"

#include "DefaultLoggerService.h"
#include "DefaultConsoleService.h"

namespace flappy {

DebugServices::DebugServices()
    : m_console(std::make_shared<DefaultConsoleService>())
    , m_logger(std::make_shared<DefaultLoggerService>(m_console))
{
}

} // flappy
