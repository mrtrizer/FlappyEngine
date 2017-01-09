#include "CameraBuilder.h"

#include <CameraComponent.h>

namespace flappy {

void CameraBuilder::build(std::shared_ptr<Entity> entity) const {
    if ((m_size.x < 1) || (m_size.y < 1))
        throw std::runtime_error("Camera size should be greater or equal 1 pixel");

    auto camera = entity->create<CameraComponent>();
    camera->setHeight(m_size.y);
}

} // flappy
