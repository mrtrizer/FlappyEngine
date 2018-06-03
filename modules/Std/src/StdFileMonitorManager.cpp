#include "StdFileMonitorManager.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#else
#define stat _stat
#endif

#include <Tools.h>

namespace flappy {

unsigned long StdFileMonitorManager::getLastModificationTime(std::string filePath) {
    struct stat fileStat;
    int result = stat(filePath.c_str(), &fileStat);
    if(result == 0) {
#ifdef __APPLE__
        return fileStat.st_mtimespec.tv_sec;
#else
        return fileStat.st_mtim.tv_sec;
#endif
    } else {
        LOGE("Can't get last modification time for \"%s\". Error: %d", filePath.c_str(), result);
        return 0;
    }
}

void StdFileMonitorManager::registerFile(std::string path) {
    m_fileInfoMap[path] = getLastModificationTime(path);
}

bool StdFileMonitorManager::changed(std::string path) {
    bool changed = false;

    auto iter = m_fileInfoMap.find(path);

    if (iter != m_fileInfoMap.end()) {
        auto lastFileTime = iter->second;

        unsigned long lastModificationTime = getLastModificationTime(path);

        if ((lastFileTime != lastModificationTime) && (lastModificationTime != 0)) {
            iter->second = lastModificationTime;
            changed = true;
        }
    } else {
        registerFile(path);
    }

    return changed;
}

bool StdFileMonitorManager::exists(std::string path) {
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}

} // flappy
