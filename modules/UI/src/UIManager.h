#pragma once

#include <Manager.h>

namespace flappy {

class TouchComponent;

class UIManager: public Manager<UIManager>
{
public:
    UIManager();
    void registerTouchComponent(SafePtr<TouchComponent> touchComponent);
    void unregisterTouchComponent(SafePtr<TouchComponent> touchComponent);

private:
    SafePtr<TouchComponent> findTouchComponentInPoint(glm::vec2 point);

    std::list<SafePtr<TouchComponent>> m_touchComponentList;
};

} // flappy
