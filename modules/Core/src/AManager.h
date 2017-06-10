#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class AManager: public Component
{
public:

    using Component::Component;

    virtual int managerId() = 0;

protected:


    void postEvent(EventHandle&& eventHandle);
};

} // flappy
