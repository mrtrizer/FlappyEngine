#include "Button.h"

#include <TransformComponent.h>
#include <PresenterComponent.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <Scene.h>

namespace flappy {

void Button::update(TimeDelta) {
    if (MGR<InputManager>()->mouseDown()) {
        if (isInField()) {
            if (m_onClick != nullptr)
                m_onClick();
            entity()->get<TransformComponent>()->setScale(0.95f);
        }
    }
    if (MGR<InputManager>()->mouseUp()) {
        if (isInField()) {
            entity()->get<TransformComponent>()->setScale(1.0f);
        }
    }
}

bool Button::isInField() {
    if (auto scene = MGR<SceneManager>()->scene()) {
        auto pos = scene->camera()->screenToScene(MGR<InputManager>()->mousePos());
        auto buttonPos = entity()->get<TransformComponent>()->pos();
        auto size = 0.5f * entity()->transform()->scale();
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
