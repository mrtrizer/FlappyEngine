#pragma once

#include <Component.h>
#include <Entity.h>

namespace flappy {

class EventRedirectComponent : public Component
{
public:
    template <typename InputEventType, typename OutputEventType>
    void redirectToRoot(const OutputEventType& event) {
        events()->subscribe([this, event](InputEventType) {
            entity()->root()->events()->post(event);
        });
    }
};

} // flappy
