#include "DefaultConsoleService.h"

#include <unordered_map>
#include <iostream>

namespace flappy {

void DefaultConsoleService::print(LogMessageType messageType, const char* str) {
    std::unordered_map<LogMessageType, std::string> messageTypeMap {
        {LogMessageType::TRACE, " [TRACE] "},
        {LogMessageType::INFO, "\x1b[32m [INFO] \x1b[0m"},
        {LogMessageType::WARNING, "\x1b[33m [WARNING] \x1b[0m"},
        {LogMessageType::ERROR, "\x1b[31m [ERROR] \x1b[0m"},
    };

    std::cout << messageTypeMap[messageType] << str << std::endl;
}

} // flappy
