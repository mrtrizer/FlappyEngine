#include "ButtonBuilder.h"

#include <ButtonComponent.h>
#include <SpriteComponent.h>
#include <SizeComponent.h>
#include <ResManager.h>

namespace flappy {

std::shared_ptr<Entity> ButtonBuilder::build() const {
    auto entity = std::make_shared<Entity>();

    if (m_idlePath.empty())
        throw std::runtime_error("Sprite path for idle state is not defined.");
    auto buttonComponent = entity->component<ButtonComponent>();
    auto quad = manager<ResManager>()->getRes<QuadRes>(m_idlePath);
    entity->component<SpriteComponent>()
            ->setQuad(quad);
    entity->component<SizeComponent>()
            ->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));

    return entity;
}

} // flappy
