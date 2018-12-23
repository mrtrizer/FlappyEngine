#include "SpriteComponent.h"

#include "Render.h"

namespace flappy {

SpriteComponent::SpriteComponent(Handle<Hierarchy> hierarchy)
    : m_renderElementFactory(hierarchy->manager<RenderElementFactory>())
{}
    
void SpriteComponent::setEntity(Handle<Entity> entity) {
    m_entity = entity;
    m_renderElement = m_renderElementFactory->createSpriteRender(entity);
}
    
SpriteComponent::~SpriteComponent() {
    m_entity->removeComponent(m_renderElement);
}

std::shared_ptr<SpriteRes> SpriteComponent::quadRes() {
    if (m_quadRes != nullptr && m_quadRes->resUpdated()) {
        m_quadRes = m_quadRes->lastRes();
    }
    return m_quadRes;
}

} // flappy
