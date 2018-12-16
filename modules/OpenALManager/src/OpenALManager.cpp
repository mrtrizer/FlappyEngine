#include "OpenALManager.h"

#include <al.h>
#include <alc.h>

#include <Entity.hpp>
#include <Hierarchy.hpp>
#include <TransformComponent.h>
#include <OpenALListenerComponent.h>

#include "OpenALUtils.h"

namespace flappy {

OpenALManager::OpenALManager()
{
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
}
    
    OpenALManager::~OpenALManager() {
        std::cout << "test";
    };

void OpenALManager::registerListener(Handle<OpenALListenerComponent> listener) {
    auto transform = listener->entity()->component<TransformComponent>();
    auto position = transform->pos();
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    CHECK_AL_ERROR;
}

void OpenALManager::unregisterListener(Handle<OpenALListenerComponent> listener) {
    if (m_listener == listener) {
        m_listener = nullptr;
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        CHECK_AL_ERROR;
    }
}

} // flappy
