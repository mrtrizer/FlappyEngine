#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class IManager: public Component
{
public:

    using Component::Component;

    virtual unsigned managerId() = 0;

protected:


    void postEvent(EventHandle&& eventHandle);
};

} // flappy
