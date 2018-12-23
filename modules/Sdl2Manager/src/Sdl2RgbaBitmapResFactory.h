#pragma once

#include <string>

#include <ResFactory.h>
#include <IRgbaBitmapRes.h>
#include <Handle.hpp>

namespace flappy {

class Hierarchy;
class ResRepositoryManager;
class IFileMonitorManager;
    
class Sdl2RgbaBitmapResFactory :public ResFactory<IRgbaBitmapRes>
{
public:
    Sdl2RgbaBitmapResFactory(Handle<Hierarchy> hierarchy);
    std::shared_ptr<ResBase> load(const std::string&, ExecType) final;
    std::shared_ptr<ResBase> create(const std::string&) final;
    bool changed(const std::string&) final;
    
private:
    Handle<ResRepositoryManager> m_resRepositoryManager;
    Handle<IFileMonitorManager> m_fileMonitorManager;
};

} // flappy
