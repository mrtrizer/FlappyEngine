#pragma once

#include <unordered_map>
#include <string>

#include <IFileMonitor.h>

namespace flappy {

class StdFileMonitor: public IFileMonitor
{
public:
    void registerFile(std::string path);
    bool changed(std::string path);

private:
    std::unordered_map<std::string, time_t> m_fileInfoMap;
};

} // flappy
