#pragma once

#include <al.h>

#include <Component.h>

#include "OpenALAudioRes.h"

namespace flappy {

class OpenALSourceComponent : public Component<OpenALSourceComponent> {
public:
    OpenALSourceComponent();

    void setAudioRes(std::shared_ptr<OpenALAudioRes> audioRes);
    void play();

private:
    ALuint m_sourceId = 0;
    std::shared_ptr<OpenALAudioRes> m_audioRes;

    bool isReadyToPlay();
};

} // flappy
