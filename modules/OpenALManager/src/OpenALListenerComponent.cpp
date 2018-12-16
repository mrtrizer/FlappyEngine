#include "OpenALListenerComponent.h"

#include <TransformComponent.h>
#include <Entity.hpp>
#include <Hierarchy.hpp>

#include "OpenALManager.h"

namespace flappy {

    OpenALListenerComponent::OpenALListenerComponent(Handle<Entity> entity)
        : m_entity(entity)
        , m_openALManager(entity->hierarchy()->manager<OpenALManager>())
    {
        m_openALManager->registerListener(selfHandle());
    }
    
    OpenALListenerComponent::~OpenALListenerComponent() {
        m_openALManager->unregisterListener(selfHandle());
    }

}
