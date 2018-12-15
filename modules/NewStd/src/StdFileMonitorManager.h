#pragma once

#include <unordered_map>
#include <string>

#include <IFileMonitorManager.h>

namespace flappy {

class StdFileMonitorManager: public IFileMonitorManager
{
public:
    void registerFile(std::string path) override;
    bool changed(std::string path) override;
    bool exists(std::string path) override;

private:
    std::unordered_map<std::string, time_t> m_fileInfoMap;

    unsigned long getLastModificationTime(std::string filePath);
};

} // flappy
