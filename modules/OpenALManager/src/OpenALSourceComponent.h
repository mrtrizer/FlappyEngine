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
    void stop();

    void setLooping(bool looping);
    void setPitch(float pitch);
    void setGain(float gain);
    bool looping();

private:
    ALuint m_sourceId = 0;
    std::shared_ptr<OpenALAudioRes> m_audioRes;
    bool m_looping = false;
    float m_pitch = 1.0f;
    float m_gain = 1.0f;

    bool isPlaying();
    bool isReadyToPlay();
};

} // flappy
