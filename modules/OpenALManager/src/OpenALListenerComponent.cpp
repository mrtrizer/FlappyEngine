#include "OpenALListenerComponent.h"

#include <TransformComponent.h>

#include "OpenALManager.h"

namespace flappy {

OpenALListenerComponent::OpenALListenerComponent() {

    addDependency(OpenALManager::id());
    addDependency(TransformComponent::id());

    events()->subscribe([this](InitEvent) {
        manager<OpenALManager>()->registerListener(selfPointer());
    });

    events()->subscribe([this](InitEvent) {
        manager<OpenALManager>()->unregisterListener(selfPointer());
    });
}

}
