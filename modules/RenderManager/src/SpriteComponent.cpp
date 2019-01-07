#include "SpriteComponent.h"

#include "Render.h"

namespace flappy {

SpriteComponent::SpriteComponent(Handle<Entity> entity)
    : m_renderElementFactory(entity->hierarchy()->manager<RenderElementFactory>())
{}
    
void SpriteComponent::addedToEntity(Handle<Entity> entity) {
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
