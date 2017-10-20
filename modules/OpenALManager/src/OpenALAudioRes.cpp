#include "OpenALAudioRes.h"

#include <Entity.h>

#include "OpenALManager.h"
#include "OpenALUtils.h"

namespace flappy {

OpenALAudioRes::OpenALAudioRes(SafePtr<Entity> rootEntity, std::vector<char> &&data, ALenum format, ALsizei freq)
    : m_rootEntity(rootEntity)
    , m_buffer(std::move(data))
    , m_format(format)
    , m_freq(freq)
{
    m_rootEntity->events()->subscribe([this](const ManagerBase::ManagerRemovedEvent& e) {
        if (e.id == OpenALManager::id())
            deinitOpenALBuffer();
    });
}

ALuint OpenALAudioRes::bufferId() {
    initOpenALBuffer();
    return m_bufferId;
}

void OpenALAudioRes::deinitOpenALBuffer() {
    if (m_bufferId != 0) {
        alDeleteBuffers(1, &m_bufferId);
        CHECK_AL_ERROR;
        m_bufferId = 0;
    }
}

void OpenALAudioRes::initOpenALBuffer() {
    if (m_bufferId == 0) {
        alGenBuffers(1, &m_bufferId);
        alBufferData(m_bufferId, m_format, m_buffer.data(), static_cast<ALsizei>(m_buffer.size()), m_freq);
        CHECK_AL_ERROR;
    }
}

}
