#pragma once

#include <Component.h>
#include <ManagerList.h>

namespace flappy {

class EventRedirectComponent : public Component
{
public:
    template <typename InputEventType, typename OutputEventType>
    void redirectToManagerList(const OutputEventType& event) {
        events()->subscribe([this, event](InputEventType) {
            managerList().lock()->events()->post(event);
        });
    }
};

} // flappy
