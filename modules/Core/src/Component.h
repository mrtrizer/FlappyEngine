#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>
#include <SafePtr.h>
#include <Tools.h>

#include "ComponentBase.h"

namespace flappy
{

class ManagerBase;
template <typename DerivedT>
class Manager;
class Entity;
class EventController;

template <typename DerivedT>
class Component: public ComponentBase
{
public:
    using ComponentBase::ComponentBase;

    unsigned componentId() override {
        return ClassId<ComponentBase, DerivedT>::id();
    }

    static unsigned id() {
        return ClassId<ComponentBase, DerivedT>::id();
    }
};

} // flappy
