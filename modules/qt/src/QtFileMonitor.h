#pragma once

#include <unordered_map>
#include <string>

#include <QDateTime>

#include <IFileMonitor.h>

namespace flappy {

class QtFileMonitor: public IFileMonitor
{
public:
    QtFileMonitor() = default;
    virtual ~QtFileMonitor() = default;
    QtFileMonitor(const QtFileMonitor&) = delete;
    QtFileMonitor& operator=(const QtFileMonitor&) & = delete;
    QtFileMonitor(QtFileMonitor&&) = delete;
    QtFileMonitor& operator=(QtFileMonitor&&) & = delete;

    void registerFile(std::string path);
    bool changed(std::string path);

private:
    std::unordered_map<std::string, QDateTime> m_fileInfoMap;
};

} // flappy
