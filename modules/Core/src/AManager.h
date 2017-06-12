#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class AManager: public Component
{
public:

    using Component::Component;

    virtual unsigned managerId() = 0;

protected:


    void postEvent(EventHandle&& eventHandle);
};

} // flappy
