#pragma once

#include <fstream>
#include <sstream>

namespace flappy {

class IFileMonitorManager;
class TextRes;

class [[manager]] IFileLoadManager {
public:
    virtual std::string loadTextFile(const std::string& path) = 0;
};

}
