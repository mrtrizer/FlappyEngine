#include "ButtonComponent.h"

#include <TransformComponent.h>
#include <PresenterComponent.h>
#include <SizeComponent.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <Scene.h>

namespace flappy {

void ButtonComponent::update(TimeDelta) {
    if (manager<InputManager>()->mouseDown()) {
        if (isInField()) {
            events()->post(OnButtonPress());
            m_mouseDown = true;
        }
    }
    if (manager<InputManager>()->mouseUp()) {
        if (isInField() && m_mouseDown) {
            events()->post(OnButtonRelease());
            events()->post(OnButtonClick());
        }
        m_mouseDown = false;
    }
}

bool ButtonComponent::isInField() {
    if (auto scene = manager<SceneManager>()->scene()) {
        auto pos = scene->camera()->screenToScene(manager<InputManager>()->mousePos());
        auto buttonPos = entity()->transform()->pos();
        auto buttonSize = entity()->component<SizeComponent>()->size();
        auto size = 0.5f * buttonSize * entity()->transform()->scale();
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
