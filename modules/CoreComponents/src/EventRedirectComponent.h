#pragma once

#include <Component.h>

namespace flappy {

class EventRedirectComponent : public Component
{
public:
    template <typename InputEventType, typename OutputEventType>
    void redirectToManagerList(const OutputEventType& event) {
        events()->subscribe([this, event](InputEventType) {
            managerList()->events()->post(event);
        });
    }
};

} // flappy
