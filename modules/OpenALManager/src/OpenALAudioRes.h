#pragma once

#include <vector>

#include <al.h>

#include <Res.h>

namespace flappy {

class OpenALAudioRes: public Res<OpenALAudioRes> {
public:
    OpenALAudioRes(std::vector<char>&& data, ALenum format, ALsizei freq);

    ALuint bufferId();

private:
    std::vector<char> m_buffer;
    ALenum m_format = AL_FORMAT_MONO8;
    ALsizei m_freq = 0;
    ALuint m_bufferId = 0;

    void deinitOpenALBuffer();
    void initOpenALBuffer();
};

} // flappy
