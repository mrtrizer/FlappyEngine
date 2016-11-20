#include "QtFileMonitor.h"

#include <QFileInfo>

namespace flappy {

using namespace std;

void QtFileMonitor::registerFile(string path) {
    m_fileInfoMap[path] = QFileInfo(QString::fromStdString(path)).lastModified();
}

bool QtFileMonitor::changed(string path) {
    auto iter = m_fileInfoMap.find(path);
    if (iter == m_fileInfoMap.end())
        return false;

    auto& lastFileTime = iter->second;
    auto fileInfo = QFileInfo(QString::fromStdString(path));
    if (lastFileTime != fileInfo.lastModified()) {
        iter->second = fileInfo.lastModified();
        return true;
    }
    return false;
}

} // flappy
