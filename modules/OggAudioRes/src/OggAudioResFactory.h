#pragma once

#include <Manager.h>
#include <ResFactory.h>
#include <OpenALAudioRes.h>

namespace flappy {

class OggAudioResFactory: public ResFactory<OpenALAudioRes> {
public:
    OggAudioResFactory();

    std::shared_ptr<ResBase> load(const std::string& resInfo, ExecType execType);
    std::shared_ptr<ResBase> create(const std::string& name);
    bool changed(const std::string& name);
};

} // flappy
