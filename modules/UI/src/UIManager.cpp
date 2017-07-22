#include "UIManager.h"

#include <TouchInputManager.h>

namespace flappy {

UIManager::UIManager():Manager({TouchInputManager::id()}) {
    events()->subscribeIn([](TouchInputManager::TouchDownEvent e) {

    });
    events()->subscribeIn([](TouchInputManager::TouchUpEvent e) {

    });
    events()->subscribeIn([](TouchInputManager::TouchMoveEvent e) {

    });
}

void UIManager::registerTouchComponent(SafePtr<TouchComponent> touchComponent)
{

}

void UIManager::unregisterTouchComponent(SafePtr<TouchComponent> touchComponent)
{

}

} // flappy
