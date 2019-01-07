#pragma once

#include <fstream>
#include <sstream>

#include <IFileLoadManager.h>
#include <ReflectionMarkers.hpp>

namespace flappy {

class TextRes;

class REFLECT StdFileLoadManager: public IFileLoadManager {
public:
    std::string loadTextFile(const std::string& filePath) final;
};

}
