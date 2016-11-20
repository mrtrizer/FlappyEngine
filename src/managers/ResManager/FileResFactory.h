#pragma once

#include "ResFactory.h"

namespace flappy {

class IFileMonitor;

class FileResFactory : public ResFactory
{
public:
    FileResFactory(std::string path = "", std::string ext = "", std::shared_ptr<IFileMonitor> fileMonitor = nullptr);

    std::shared_ptr<Res> load(const std::string& name, std::shared_ptr<ResManager>) override;
    virtual std::shared_ptr<Res> loadFile(const std::string& path) = 0;
    bool changed(const std::string&) override;

private:
    std::string m_path;
    std::string m_ext;
    std::shared_ptr<IFileMonitor> m_fileMonitor;
};

} // flappy
