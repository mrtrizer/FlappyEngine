#include "TransformComponent.h"

namespace flappy {

using namespace glm;

void TransformComponent::init() {
    entity()->m_transform = shared_from_this();
    for (auto tmpChild: m_tmpChildren)
        manager<EntityManager>()->add(tmpChild);
    m_tmpChildren.clear();
}

mat4x4 TransformComponent::getMvMatrix() {
    auto translateM = translate(mat4x4(1.0f), m_pos);
    auto rotateM = glm::rotate(translateM, m_angle, vec3(0.0f, 0.0f, 1.0f));
    auto scaleM = glm::scale(rotateM, m_scale);
    return scaleM;
}

std::shared_ptr<Entity> TransformComponent::addChild(std::shared_ptr<Entity> entity) {
    if (managerList() != nullptr)
        manager<EntityManager>()->add(entity);
    else
        m_tmpChildren.push_back(entity);
    entity->transform()->setParent(shared_from_this());
    return entity;
}

std::shared_ptr<Entity> TransformComponent::createChild() {
    return addChild(std::make_shared<Entity>());
}

} // flappy
