#include "ButtonBuilder.h"

#include <ButtonComponent.h>
#include <SpriteComponent.h>
#include <SizeComponent.h>
#include <ResManager.h>

namespace flappy {

void ButtonBuilder::build(std::shared_ptr<Entity> entity) const {
    if (m_idlePath.empty())
        throw std::runtime_error("Sprite path for idle state is not defined.");
    auto buttonComponent = entity->create<ButtonComponent>();
    if (m_onClickHandler != nullptr) {
        entity->events()->subscribe(buttonComponent, [onClick = m_onClickHandler](ButtonComponent::OnButtonClick e) {
            onClick();
        });
    }
    auto quad = manager<ResManager>()->getRes<QuadRes>(m_idlePath);
    entity->create<SpriteComponent>()
            ->setQuad(quad);
    entity->create<SizeComponent>()
            ->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));
}

} // flappy
