#pragma once

#include <glm/vec2.hpp>

#include <Handle.hpp>
#include <ISubscription.h>

namespace flappy {

class TouchComponent;
class Hierarchy;

class REFLECT UIManager
{
public:
    UIManager(Handle<Hierarchy> hierarchy);
    void registerTouchComponent(Handle<TouchComponent> touchComponent);
    void unregisterTouchComponent(Handle<TouchComponent> touchComponent);

private:
    Handle<TouchComponent> findTouchComponentInPoint(glm::vec2 point);

    std::list<Handle<TouchComponent>> m_touchComponentList;
    std::shared_ptr<ISubscription> m_downSubscription;
    std::shared_ptr<ISubscription> m_upSubscription;
    std::shared_ptr<ISubscription> m_moveSubscription;
};

} // flappy
