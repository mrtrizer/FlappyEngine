#pragma once

#include <Component.h>
#include <View.h>

#include "RenderElementFactory.h"

namespace flappy {

class MeshComponent: public Component<MeshComponent> {
public:
    MeshComponent()
    {
        addDependency(RenderElementFactory::id());

        subscribe([this](InitEvent) {
            m_renderElement = manager<RenderElementFactory>()->createMeshRender(selfPointer());
            entity()->addComponent(m_renderElement);
        });

        subscribe([this](DeinitEvent) {
            entity()->removeComponent(m_renderElement);
            m_renderElement.reset();
        });
    }

private:
    std::shared_ptr<View> m_renderElement;
};

} // flappy
