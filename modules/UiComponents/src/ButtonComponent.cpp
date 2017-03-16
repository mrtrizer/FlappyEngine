#include "ButtonComponent.h"

#include <TransformComponent.h>
#include <PresenterComponent.h>
#include <SizeComponent.h>
#include <InputManager.h>
#include <SceneManager.h>

namespace flappy {

void ButtonComponent::update(DeltaTime) {
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
    assert(false);
    return false;
}

} // flappy
