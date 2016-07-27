#include "button.h"

#include <core/scenemgr.h>
#include <core/transform.h>
#include <core/inputmgr.h>
#include <core/presenter.h>

void Button::update(TimeDelta) {
    if (Input::isMouseDown()) {
        if (isInField()) {
            if (onClick != nullptr)
                onClick();
            entity()->get<Transform>()->setScale(0.95f);
        }
    }
    if (Input::isMouseUp()) {
        if (isInField()) {
            entity()->get<Transform>()->setScale(1.0f);
        }
    }
}

bool Button::isInField() {
    auto pos = Scene::camera()->screenToScene(Input::getMousePos());
    auto buttonPos = entity()->get<Transform>()->pos();
    auto size = entity()->get<Presenter>()->size() * 0.5f;
    auto diff = pos - buttonPos;
    return (glm::abs(diff.x) < size.x) && (glm::abs(diff.y) < size.y);
}
