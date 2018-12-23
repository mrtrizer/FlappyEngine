#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>
#include <ShaderRes.h>
#include <Handle.hpp>

namespace flappy {

class Hierarchy;
class IFileLoadManager;
class IFileMonitorManager;
class ResRepositoryManager;
    
class GLShaderResFactory: public ResFactory<ShaderRes> {
public:
    GLShaderResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
    
private:
    Handle<IFileLoadManager> m_fileLoadManager;
    Handle<IFileMonitorManager> m_fileMonitorManager;
    Handle<ResRepositoryManager> m_resRepositoryManager;
    Handle<Entity> m_rootEntity;
};

} // flappy
