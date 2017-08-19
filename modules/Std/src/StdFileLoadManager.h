#pragma once

#include <fstream>
#include <sstream>

#include <IFileLoadManager.h>

namespace flappy {

class TextRes;

class StdFileLoadManager: public IFileLoadManager {
public:
    StdFileLoadManager(std::string path);
    std::string loadTextFile(const std::string& filePath) override;

private:
    std::string m_path;
};

}
