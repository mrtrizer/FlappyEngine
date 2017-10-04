#include "OpenALSourceComponent.h"

#include <Entity.h>
#include <TransformComponent.h>

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
        auto transformComponent = entity()->findComponent<TransformComponent>();
        if (transformComponent != nullptr) {
            auto& pos = transformComponent->pos();
            alSource3f(m_sourceId, AL_POSITION, pos.x, pos.y, pos.z);
        }
        if ((m_audioRes != nullptr) && (m_audioRes->nextRes() != m_audioRes)) {
            m_audioRes = m_audioRes->lastRes();
            bool needRestart = isPlaying();
            if (needRestart)
                alSourceStop(m_sourceId);
            alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
            alSourcei(m_sourceId, AL_LOOPING, m_looping?AL_TRUE:AL_FALSE);
            alSourcef(m_sourceId, AL_PITCH, m_pitch);
            alSourcef(m_sourceId, AL_GAIN, m_gain);
            if (needRestart)
                alSourcePlay(m_sourceId);
        }
    });
}

bool OpenALSourceComponent::isPlaying() {
    if (m_sourceId == 0)
        return false;
    ALint state;
    alGetSourcei(m_sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool OpenALSourceComponent::isReadyToPlay() {
    return (m_audioRes != nullptr) && isInitialized();
}

void OpenALSourceComponent::setAudioRes(std::shared_ptr<OpenALAudioRes> audioRes) {
    m_audioRes = audioRes;
    if (isReadyToPlay()) {
        alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
    }
}

void OpenALSourceComponent::play() {
    if (isReadyToPlay())
        alSourcePlay(m_sourceId);
}

void OpenALSourceComponent::stop() {
    if (isReadyToPlay())
        alSourceStop(m_sourceId);
}

void OpenALSourceComponent::setLooping(bool looping)
{
    m_looping = looping;
    if (isInitialized()) {
        alSourcei(m_sourceId, AL_LOOPING, looping?AL_TRUE:AL_FALSE);
    }
}

void OpenALSourceComponent::setPitch(float pitch)
{
    m_pitch = pitch;
    if (isInitialized()) {
        alSourcef(m_sourceId, AL_PITCH, pitch);
    }
}

void OpenALSourceComponent::setGain(float gain)
{
    m_gain = gain;
    if (isInitialized()) {
        alSourcef(m_sourceId, AL_GAIN, gain);
    }
}

}
