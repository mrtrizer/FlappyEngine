#include "OpenALSourceComponent.h"

#include "OpenALManager.h"

namespace flappy {

OpenALSourceComponent::OpenALSourceComponent() {
    addDependency(OpenALManager::id());
    events()->subscribe([this](InitEvent) {
        alGenSources(1, &m_sourceId);
        alSource3f(m_sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
    });

    events()->subscribe([this](DeinitEvent) {
        alDeleteSources(1, &m_sourceId);
        m_sourceId = 0;
    });

    events()->subscribe([this](UpdateEvent) {
        if ((m_audioRes != nullptr) && (m_audioRes->nextRes() != m_audioRes)) {
            m_audioRes = m_audioRes->lastRes();
            alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
        }
    });
}

bool OpenALSourceComponent::isReadyToPlay() {
    return (m_audioRes != nullptr) && isInitialized();
}

void OpenALSourceComponent::setAudioRes(std::shared_ptr<OpenALAudioRes> audioRes) {
    m_audioRes = audioRes;
    if (isReadyToPlay())
        alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
}

void OpenALSourceComponent::play() {
    if (isReadyToPlay())
        alSourcePlay(m_sourceId);
}

}
