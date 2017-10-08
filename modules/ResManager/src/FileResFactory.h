#pragma once

#include <string>

#include "ResFactory.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"
#include "ResRepositoryManager.h"
#include "TextRes.h"

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
class FileResFactory : public ResFactory<TextRes>
{
public:
    FileResFactory();

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
};

/// @}

} // flappy
