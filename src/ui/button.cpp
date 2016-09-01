#include "button.h"

#include <core/scenemgr.h>
#include <core/transform.h>
#include <core/inputmgr.h>
#include <core/presenter.h>

namespace flappy {

void Button::update(TimeDelta) {
    if (MGR<InputMgr>()->isMouseDown()) {
        if (isInField()) {
            if (onClick != nullptr)
                onClick();
            entity()->get<Transform>()->setScale(0.95f);
        }
    }
    if (MGR<InputMgr>()->isMouseUp()) {
        if (isInField()) {
            entity()->get<Transform>()->setScale(1.0f);
        }
    }
}

bool Button::isInField() {
    if (auto scene = MGR<SceneMgr>()->scene()) {
        auto pos = scene->camera()->screenToScene(MGR<InputMgr>()->getMousePos());
        auto buttonPos = entity()->get<Transform>()->pos();
        auto size = entity()->get<Presenter>()->size() * 0.5f;
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
