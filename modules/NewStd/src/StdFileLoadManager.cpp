#include "StdFileLoadManager.h"

#include <TextRes.h>
#include <IFileMonitorManager.h>

namespace flappy {

std::string StdFileLoadManager::loadTextFile(const std::string& filePath) {
    std::ifstream fStream(filePath);
    std::stringstream sStream;
    sStream << fStream.rdbuf();
    fStream.close();
    return sStream.str();
}

}
