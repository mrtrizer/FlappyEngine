#include "OpenALManager.h"

#include <al.h>
#include <alc.h>

#include <ThreadManager.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <OpenALListenerComponent.h>

namespace flappy {

OpenALManager::OpenALManager()
{
    events()->subscribe([this](InitEvent) {
        ALCdevice *device;
        device = alcOpenDevice(NULL);
        if (!device)
            throw std::runtime_error("Can't create a device ");

        auto context = alcCreateContext(device, NULL);

        if (!alcMakeContextCurrent(context))
            throw std::runtime_error("Can't make a context current");

        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    });

}

void OpenALManager::registerListener(SafePtr<OpenALListenerComponent> listener) {
    auto transform = listener->entity()->component<TransformComponent>();
    auto position = transform->pos();
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void OpenALManager::unregisterListener(SafePtr<OpenALListenerComponent> listener) {
    if (m_listener == listener) {
        m_listener = SafePtr<OpenALListenerComponent>();
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    }
}

bool OpenALManager::isReady() const {
    return entity()->component<ThreadManager>() != nullptr;
}

} // flappy
