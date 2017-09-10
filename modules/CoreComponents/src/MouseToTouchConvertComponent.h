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
    SafePtr<ISubscription> m_mouseDownSubscription;
    SafePtr<ISubscription> m_mouseUpSubscription;
    SafePtr<ISubscription> m_mouseMoveSubscription;
};

} // flappy
