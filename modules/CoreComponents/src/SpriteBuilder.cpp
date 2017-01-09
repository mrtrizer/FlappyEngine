#include "SpriteBuilder.h"

#include <SpriteComponent.h>
#include <ResManager.h>
#include <SizeComponent.h>

namespace flappy {

void SpriteBuilder::build(std::shared_ptr<Entity> entity) const {
    if (m_spritePath.empty())
        throw std::runtime_error("Sprite path is not defined.");

    auto quad = manager<ResManager>()->getRes<QuadRes>(m_spritePath);

    auto sprite = entity->create<SpriteComponent>();
    sprite->setQuad(quad);

    auto sizeComponent = entity->create<SizeComponent>();
    sizeComponent->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));
}

} // flappy
