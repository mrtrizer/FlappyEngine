#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class AManager: public Component
{
public:
    struct OnManager: public IEvent
    {
        unsigned id;
        SafePtr<AManager> pointer;
        template <typename ManagerT>
        SafePtr<ManagerT> castTo() {
            if (ClassId<Component, ManagerT>::id() == id)
                return SafePtr<ManagerT>(pointer);
            else
                return SafePtr<ManagerT>();
        }
    };

    struct OnManagerAdded: public OnManager
    {};

    struct OnManagerRemoved: public OnManager
    {};

    virtual int managerId() = 0;

protected:
    void postEvent(EventHandle&& eventHandle);
};

} // flappy
