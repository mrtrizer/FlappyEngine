#pragma once

#include <ReflectionMarkers.hpp>

namespace flappy {

class REFLECT IFileMonitorManager
{
public:
    IFileMonitorManager() = default;
    virtual ~IFileMonitorManager() = default;
    IFileMonitorManager(const IFileMonitorManager&) = delete;
    IFileMonitorManager& operator=(const IFileMonitorManager&) & = delete;
    IFileMonitorManager(IFileMonitorManager&&) = delete;
    IFileMonitorManager& operator=(IFileMonitorManager&&) & = delete;

    virtual void registerFile(std::string path) = 0;
    virtual bool changed(std::string path) = 0;
    virtual bool exists(std::string path) = 0;
};

} // flappy
