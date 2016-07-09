#include "button.h"

#include <core/scene.h>
#include <core/transform.h>
#include <core/input.h>
#include <core/presenter.h>

void Button::update(TimeDelta) {
    if (Input::getInst()->isMouseDown()) {
        if (isInField()) {
            if (onClick != nullptr)
                onClick();
            getEntity()->get<Transform>()->scale = 0.95f;
        }
    }
    if (Input::getInst()->isMouseUp()) {
        if (isInField()) {
            getEntity()->get<Transform>()->scale = 1.0f;
        }
    }
}

bool Button::isInField() {
    auto pos = Scene::getInst()->getCamera()->screenToScene(Input::getInst()->getMousePos());
    auto buttonPos = getEntity()->get<Transform>()->pos;
    auto size = getEntity()->get<Presenter>()->getSize() * 0.5f;
    auto diff = pos - buttonPos;
    return (glm::abs(diff.x) < size.x) && (glm::abs(diff.y) < size.y);
}
