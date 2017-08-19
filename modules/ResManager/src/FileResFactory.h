#pragma once

#include <string>

#include "IResFactory.h"

namespace flappy {

class IFileMonitorManager;
class Entity;

/// @addtogroup ResManager
/// @{

/// Base class for file based resource factory.
/// @details File based resources have no dependencies.
/// Usially, they also supports function of auto-update.
/// To support auto-update of resource, pass implementation
/// of file monitor to constructor.
class FileResFactory : public IResFactory
{
public:
    FileResFactory(std::string path = "", std::string ext = "");

    std::shared_ptr<Res> load(const ResInfo& resInfo, SafePtr<Entity>) override;
    std::shared_ptr<Res> create(const std::string& name, SafePtr<Entity>) override;

private:
    std::string m_path;
    std::string m_ext;
};

/// @}

} // flappy
