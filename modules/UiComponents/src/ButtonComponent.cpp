#include "ButtonComponent.h"

#include <TransformComponent.h>
#include <PresenterComponent.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <Scene.h>

namespace flappy {

void ButtonComponent::update(TimeDelta) {
    if (MGR<InputManager>()->mouseDown()) {
        if (isInField()) {
            m_mouseDown = true;
            m_baseScale = entity()->get<TransformComponent>()->scale();
            entity()->get<TransformComponent>()->setScale(m_baseScale * 0.95f);
        }
    }
    if (MGR<InputManager>()->mouseUp()) {
        if ((m_onClick != nullptr) && isInField() && m_mouseDown)
            m_onClick();
        m_mouseDown = false;
        entity()->get<TransformComponent>()->setScale(m_baseScale);
    }
}

bool ButtonComponent::isInField() {
    if (auto scene = MGR<SceneManager>()->scene()) {
        auto pos = scene->camera()->screenToScene(MGR<InputManager>()->mousePos());
        auto buttonPos = entity()->transform()->pos();
        auto buttonSize = glm::vec3{1,1,0};
        auto size = 0.5f * buttonSize * entity()->transform()->scale();
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
