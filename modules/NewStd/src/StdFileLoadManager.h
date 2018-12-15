#pragma once

#include <fstream>
#include <sstream>

#include <IFileLoadManager.h>

namespace flappy {

class TextRes;

class StdFileLoadManager: public IFileLoadManager {
public:
    std::string loadTextFile(const std::string& filePath) final;
};

}
