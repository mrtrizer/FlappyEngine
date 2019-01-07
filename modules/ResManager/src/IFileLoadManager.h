#pragma once

#include <fstream>
#include <sstream>
#include <ReflectionMarkers.hpp>

namespace flappy {

class IFileMonitorManager;
class TextRes;

class __attribute((annotate("reflect"))) IFileLoadManager {
public:
    virtual ~IFileLoadManager() = default;
    virtual std::string loadTextFile(const std::string& path) = 0;
};

}
