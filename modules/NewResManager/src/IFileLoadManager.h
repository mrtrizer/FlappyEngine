#pragma once

#include <fstream>
#include <sstream>

namespace flappy {

class IFileMonitorManager;
class TextRes;

class [[manager]] IFileLoadManager {
public:
    virtual ~IFileLoadManager() = default;
    virtual std::string loadTextFile(const std::string& path) = 0;
};

}
