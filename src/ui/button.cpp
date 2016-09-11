#include "button.h"

#include <core/scenemanager.h>
#include <core/scene.h>
#include <core/transform.h>
#include <core/inputmanager.h>
#include <core/presenter.h>

namespace flappy {

void Button::update(TimeDelta) {
    if (MGR<InputManager>()->isMouseDown()) {
        if (isInField()) {
            if (m_onClick != nullptr)
                m_onClick();
            entity()->get<Transform>()->setScale(0.95f);
        }
    }
    if (MGR<InputManager>()->isMouseUp()) {
        if (isInField()) {
            entity()->get<Transform>()->setScale(1.0f);
        }
    }
}

bool Button::isInField() {
    if (auto scene = MGR<SceneManager>()->scene()) {
        auto pos = scene->camera()->screenToScene(MGR<InputManager>()->getMousePos());
        auto buttonPos = entity()->get<Transform>()->pos();
        auto size = 0.5f * entity()->transform()->scale();
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
