#include "StdFileLoadManager.h"

#include <TextRes.h>
#include <IFileMonitorManager.h>

namespace flappy {

StdFileLoadManager::StdFileLoadManager(std::string path)
    : m_path(path)
{}

std::string StdFileLoadManager::loadTextFile(const std::string& filePath) {
    std::ifstream fStream(m_path + "/" + filePath);
    std::stringstream sStream;
    sStream << fStream.rdbuf();
    fStream.close();
    return sStream.str();
}

}
