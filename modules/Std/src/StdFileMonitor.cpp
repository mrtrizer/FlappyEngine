#include "StdFileMonitor.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#else
#define stat _stat
#endif

namespace flappy {

using namespace std;

void StdFileMonitor::registerFile(string path) {
    struct stat result;
    if(stat(path.c_str(), &result)==0) {
        m_fileInfoMap[path] = result.st_mtim.tv_sec;
    }
}

bool StdFileMonitor::changed(string path) {
    auto iter = m_fileInfoMap.find(path);
    if (iter == m_fileInfoMap.end())
        return false;

    auto lastFileTime = iter->second;

    struct stat result;
    if(stat(path.c_str(), &result)==0) {
        if (lastFileTime != result.st_mtim.tv_sec) {
            iter->second = result.st_mtim.tv_sec;
            return true;
        }
    }

    return false;
}

} // flappy
