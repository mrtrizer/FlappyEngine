#pragma once

#include "ResFactory.h"

namespace flappy {

class IFileMonitor;

/// @addtogroup ResManager
/// @{

/// Base class for file based resource factory.
/// @details File based resources have no dependencies.
/// Usially, they also supports function of auto-update.
/// To support auto-update of resource, pass implementation
/// of file monitor to constructor.
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

/// @}

} // flappy
