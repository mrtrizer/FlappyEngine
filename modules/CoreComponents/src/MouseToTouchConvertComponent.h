#pragma once

#include <memory>

#include <Component.h>
#include <EventBus.h>

namespace flappy
{

class MouseToTouchConvertComponent: public Component<MouseToTouchConvertComponent>
{
public:
    MouseToTouchConvertComponent();

private:
    std::shared_ptr<ISubscription> m_mouseDownSubscription;
    std::shared_ptr<ISubscription> m_mouseUpSubscription;
    std::shared_ptr<ISubscription> m_mouseMoveSubscription;
};

} // flappy
