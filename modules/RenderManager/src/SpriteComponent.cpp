#include "SpriteComponent.h"

#include "Render.h"

namespace flappy {

SpriteComponent::SpriteComponent()
{
    addDependency(RenderElementFactory::id());

    subscribe([this](InitEvent) {
        m_renderElement = manager<RenderElementFactory>()->createSpriteRender(selfPointer());
        entity()->addComponent(m_renderElement);
    });

    subscribe([this](DeinitEvent) {
        entity()->removeComponent(m_renderElement);
        m_renderElement.reset();
    });
}

std::shared_ptr<SpriteRes> SpriteComponent::quadRes() {
    if (m_quadRes != nullptr && m_quadRes->resUpdated()) {
        m_quadRes = m_quadRes->lastRes();
    }
    return m_quadRes;
}

} // flappy
