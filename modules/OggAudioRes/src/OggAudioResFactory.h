#pragma once

#include <Handle.hpp>
#include <ResFactory.h>
#include <OpenALAudioRes.h>

namespace flappy {
    
class Hierarchy;
class ResRepositoryManager;
class IFileMonitorManager;

class [[manager]] OggAudioResFactory: public ResFactory<OpenALAudioRes> {
public:
    OggAudioResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& resInfo, ExecType execType);
    std::shared_ptr<ResBase> create(const std::string& name);
    bool changed(const std::string& name);
    
private:
    Handle<ResRepositoryManager> m_resRepositoryManager;
    Handle<IFileMonitorManager> m_fileMonitorManager;
};

} // flappy
