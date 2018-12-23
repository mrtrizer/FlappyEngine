#pragma once

#include <string>

#include <Handle.hpp>

#include "ResFactory.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"
#include "ResRepositoryManager.h"
#include "TextRes.h"

namespace flappy {

class Hierarchy;
class IFileMonitorManager;
class Entity;

/// @addtogroup ResManager
/// @{

class TextResFactory : public ResFactory<TextRes>
{
public:
    TextResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
    
private:
    Handle<ResRepositoryManager> m_resRepoManager;
    Handle<IFileLoadManager> m_fileLoadManager;
    Handle<IFileMonitorManager> m_fileMonitorManager;
};

/// @}

} // flappy
