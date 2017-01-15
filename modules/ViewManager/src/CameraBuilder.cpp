#include "CameraBuilder.h"

#include <CameraComponent.h>

namespace flappy {

std::shared_ptr<Entity> CameraBuilder::build() const {
    auto entity = std::make_shared<Entity>();

    if ((m_size.x < 1) || (m_size.y < 1))
        throw std::runtime_error("Camera size should be greater or equal 1 pixel");

    auto camera = entity->createComponent<CameraComponent>();
    camera->setSize(m_size);

    return entity;
}

} // flappy
