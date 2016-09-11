#include "button.h"

#include <core/scenemgr.h>
#include <core/scene.h>
#include <core/transform.h>
#include <core/inputmgr.h>
#include <core/presenter.h>

namespace flappy {

void Button::update(TimeDelta) {
    if (MGR<InputMgr>()->isMouseDown()) {
        if (isInField()) {
            if (m_onClick != nullptr)
                m_onClick();
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
        auto size = 0.5f * entity()->transform()->scale();
        auto diff = pos - buttonPos;
        return (abs(diff.x) < size.x) && (abs(diff.y) < size.y);
    } else {
        return false;
    }
}

} // flappy
