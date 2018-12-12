#pragma once

#include <memory>

#include <EventBus.h>
#include <Handle.hpp>

namespace flappy
{
class Hierarchy;

class [[component]] MouseToTouchConvertComponent
{
public:
    MouseToTouchConvertComponent(Handle<Hierarchy> hierarchy);

private:
    std::weak_ptr<ISubscription> m_mouseDownSubscription;
    std::weak_ptr<ISubscription> m_mouseUpSubscription;
    std::weak_ptr<ISubscription> m_mouseMoveSubscription;
};

} // flappy
