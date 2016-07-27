#include "button.h"

#include <core/scene.h>
#include <core/transform.h>
#include <core/inputmgr.h>
#include <core/presenter.h>

void Button::update(TimeDelta) {
    if (InputMgr::getInst()->isMouseDown()) {
        if (isInField()) {
            if (onClick != nullptr)
                onClick();
            entity()->get<Transform>()->scale = 0.95f;
        }
    }
    if (InputMgr::getInst()->isMouseUp()) {
        if (isInField()) {
            entity()->get<Transform>()->scale = 1.0f;
        }
    }
}

bool Button::isInField() {
    auto pos = Scene::getInst()->getCamera()->screenToScene(InputMgr::getInst()->getMousePos());
    auto buttonPos = entity()->get<Transform>()->pos;
    auto size = entity()->get<Presenter>()->getSize() * 0.5f;
    auto diff = pos - buttonPos;
    return (glm::abs(diff.x) < size.x) && (glm::abs(diff.y) < size.y);
}
