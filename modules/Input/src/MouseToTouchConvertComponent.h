#pragma once

#include <memory>

#include <EventBus.h>
#include <Handle.hpp>

namespace flappy
{
class Entity;

class [[component]] MouseToTouchConvertComponent
{
public:
    MouseToTouchConvertComponent(const Handle<Entity>& entity);

private:
    std::weak_ptr<ISubscription> m_mouseDownSubscription;
    std::weak_ptr<ISubscription> m_mouseUpSubscription;
    std::weak_ptr<ISubscription> m_mouseMoveSubscription;
};

} // flappy
