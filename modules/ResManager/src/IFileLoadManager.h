#pragma once

#include <fstream>
#include <sstream>

#include <Manager.h>

namespace flappy {

class IFileMonitorManager;
class TextRes;

class IFileLoadManager: public Manager<IFileLoadManager> {
public:
    virtual std::string loadTextFile(const std::string& path) = 0;
};

}
