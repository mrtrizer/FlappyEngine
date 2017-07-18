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

    TypeId<ComponentBase> componentId() override {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

    static TypeId<ComponentBase> id() {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }
};

} // flappy
