#pragma once

namespace flappy {

class IFileMonitor
{
public:
    IFileMonitor() = default;
    virtual ~IFileMonitor() = default;
    IFileMonitor(const IFileMonitor&) = delete;
    IFileMonitor& operator=(const IFileMonitor&) & = delete;
    IFileMonitor(IFileMonitor&&) = delete;
    IFileMonitor& operator=(IFileMonitor&&) & = delete;

    virtual void registerFile(std::string path) = 0;
    virtual bool changed(std::string path) = 0;
};

} // flappy
