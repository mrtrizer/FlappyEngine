#include "OpenALManager.h"

#include <al.h>
#include <alc.h>

#include <ThreadManager.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <OpenALListenerComponent.h>

#include "OpenALUtils.h"

namespace flappy {

OpenALManager::OpenALManager()
{
    events()->subscribe([this](InitEvent) {
        ALCdevice *device;
        device = alcOpenDevice(NULL);
        CHECK_AL_ERROR;
        if (!device)
            throw std::runtime_error("Can't create a device ");
        CHECK_ALC_ERROR(device);

        auto context = alcCreateContext(device, NULL);
        CHECK_ALC_ERROR(device);
        CHECK_AL_ERROR;

        auto success = alcMakeContextCurrent(context);
        CHECK_ALC_ERROR(device);
        CHECK_AL_ERROR;

        if (!success)
            throw std::runtime_error("Can't select a context");

        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        CHECK_ALC_ERROR(device);
        CHECK_AL_ERROR;
    });
}

void OpenALManager::registerListener(SafePtr<OpenALListenerComponent> listener) {
    auto transform = listener->entityRef()->component<TransformComponent>();
    auto position = transform->pos();
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    CHECK_AL_ERROR;
}

void OpenALManager::unregisterListener(SafePtr<OpenALListenerComponent> listener) {
    if (m_listener == listener) {
        m_listener = SafePtr<OpenALListenerComponent>();
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        CHECK_AL_ERROR;
    }
}

bool OpenALManager::isReady() const {
    return entity()->component<ThreadManager>() != nullptr;
}

} // flappy
