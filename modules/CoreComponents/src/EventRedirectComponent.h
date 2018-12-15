#pragma once

#include <Entity.h>

namespace flappy {

class [[component]] EventRedirectComponent
{
public:
    template <typename InputEventType, typename OutputEventType>
    void redirectToRoot(const OutputEventType& event) {
        subscribe([this, event](InputEventType) {
            entity()->root()->events()->post(event);
        });
    }
};

} // flappy
