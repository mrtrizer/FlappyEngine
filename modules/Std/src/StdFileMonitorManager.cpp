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

using namespace std;

unsigned long getLastModificationTime(string filePath) {
    struct stat fileStat;
    int result = stat(filePath.c_str(), &fileStat);
    if(result == 0) {
#ifdef __APPLE__
        return fileStat.st_mtimespec.tv_sec;
#else
        return fileStat.st_mtim.tv_sec;
#endif
    } else {
        LOGE("Can't get last modification time. Error: %", result);
        return 0;
    }
}

void StdFileMonitorManager::registerFile(string path) {
    struct stat result;

    m_fileInfoMap[path] = getLastModificationTime(path);
}

bool StdFileMonitorManager::changed(string path) {
    auto iter = m_fileInfoMap.find(path);
    if (iter == m_fileInfoMap.end()) {
        registerFile(path);
        return false;
    }

    auto lastFileTime = iter->second;

    unsigned long lastModificationTime = getLastModificationTime(path);
    if (lastFileTime != lastModificationTime) {
        iter->second = lastModificationTime;
        return true;
    }

    return false;
}

} // flappy
