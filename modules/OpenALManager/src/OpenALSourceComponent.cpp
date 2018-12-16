#include "OpenALSourceComponent.h"

#include <Entity.hpp>
#include <Hierarchy.hpp>
#include <TransformComponent.h>

#include "OpenALManager.h"

#include "OpenALUtils.h"

namespace flappy {

OpenALSourceComponent::OpenALSourceComponent(Handle<Entity> entity)
    : Updatable<OpenALSourceComponent>(entity)
    , m_transform(entity->component<TransformComponent>())
{
    USER_ASSERT(entity->hierarchy()->manager<OpenALManager>().isValid());
    
    alGenSources(1, &m_sourceId);
    CHECK_AL_ERROR;
    alSource3f(m_sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
    CHECK_AL_ERROR;
}
    
void OpenALSourceComponent::update(float dt) {
    auto& pos = m_transform->pos();
    alSource3f(m_sourceId, AL_POSITION, pos.x, pos.y, pos.z);
    CHECK_AL_ERROR;
    if ((m_audioRes != nullptr) && (m_audioRes->nextRes() != m_audioRes)) {
        m_audioRes = m_audioRes->lastRes();
        bool needRestart = isPlaying();
        if (needRestart) {
            alSourceStop(m_sourceId);
            CHECK_AL_ERROR;
        }
        alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
        CHECK_AL_ERROR;
        alSourcei(m_sourceId, AL_LOOPING, m_looping?AL_TRUE:AL_FALSE);
        CHECK_AL_ERROR;
        alSourcef(m_sourceId, AL_PITCH, m_pitch);
        CHECK_AL_ERROR;
        alSourcef(m_sourceId, AL_GAIN, m_gain);
        CHECK_AL_ERROR;
        if (needRestart) {
            alSourcePlay(m_sourceId);
            CHECK_AL_ERROR;
        }
    }
}

OpenALSourceComponent::~OpenALSourceComponent() {
    alDeleteSources(1, &m_sourceId);
    CHECK_AL_ERROR;
    m_sourceId = 0;
}
    
bool OpenALSourceComponent::isPlaying() {
    if (m_sourceId == 0)
        return false;
    ALint state;
    alGetSourcei(m_sourceId, AL_SOURCE_STATE, &state);
    CHECK_AL_ERROR;
    return state == AL_PLAYING;
}

bool OpenALSourceComponent::isReadyToPlay() {
    return m_audioRes != nullptr;
}

void OpenALSourceComponent::setAudioRes(std::shared_ptr<OpenALAudioRes> audioRes) {
    m_audioRes = audioRes;
    if (isReadyToPlay()) {
        alSourcei(m_sourceId, AL_BUFFER, m_audioRes->bufferId());
        CHECK_AL_ERROR;
    }
}

void OpenALSourceComponent::play() {
    if (isReadyToPlay()) {
        alSourcePlay(m_sourceId);
        CHECK_AL_ERROR;
    }
}

void OpenALSourceComponent::stop() {
    if (isReadyToPlay()) {
        alSourceStop(m_sourceId);
        CHECK_AL_ERROR;
    }
}

void OpenALSourceComponent::setLooping(bool looping)
{
    m_looping = looping;
    alSourcei(m_sourceId, AL_LOOPING, looping?AL_TRUE:AL_FALSE);
    CHECK_AL_ERROR;
}

void OpenALSourceComponent::setPitch(float pitch)
{
    m_pitch = pitch;
    alSourcef(m_sourceId, AL_PITCH, pitch);
    CHECK_AL_ERROR;
}

void OpenALSourceComponent::setGain(float gain)
{
    m_gain = gain;
    alSourcef(m_sourceId, AL_GAIN, gain);
    CHECK_AL_ERROR;
}

} // flappy
