#include "button.h"

#include <core/scenemanager.h>
#include <core/ctransform.h>
#include <core/inputmanager.h>
#include <core/gpresenter.h>

void Button::update(TimeDelta dt) {
    if (InputManager::getInst()->isMouseDown()) {
        if (isInField()) {
            if (onClick != nullptr)
                onClick();
            getEntity()->get<CTransform>()->scale = 0.95f;
        }
    }
    if (InputManager::getInst()->isMouseUp()) {
        if (isInField()) {
            getEntity()->get<CTransform>()->scale = 1.0f;
        }
    }
}

bool Button::isInField() {
    auto pos = SceneManager::getInst()->getCamera()->screenToScene(InputManager::getInst()->getMousePos());
    auto buttonPos = getEntity()->get<CTransform>()->pos;
    auto size = getEntity()->get<GPresenter>()->getSize() * 0.5f;
    auto diff = pos - buttonPos;
    return (glm::abs(diff.x) < size.x) && (glm::abs(diff.y) < size.y);
}
