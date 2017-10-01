#pragma once

#include <vector>

#include <al.h>

#include <Res.h>
#include <SafePtr.h>

namespace flappy {

class Entity;

class OpenALAudioRes: public Res<OpenALAudioRes> {
public:
    OpenALAudioRes(SafePtr<Entity> rootEntity, std::vector<char>&& data, ALenum format, ALsizei freq);

    ALuint bufferId();

private:
    SafePtr<Entity> m_rootEntity;
    std::vector<char> m_buffer;
    ALenum m_format = AL_FORMAT_MONO8;
    ALsizei m_freq = 0;
    ALuint m_bufferId = 0;

    void deinitOpenALBuffer();
    void initOpenALBuffer();
};

} // flappy
