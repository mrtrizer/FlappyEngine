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

    std::string componentName() const override {
        return typeName<DerivedT>();
    }

    TypeId<ComponentBase> componentId() override {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

    static TypeId<ComponentBase> id() {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

private:
    void initInternal() override {
        LOGI("Init %s", typeName<DerivedT>().c_str());

        events()->post(InitEvent());
        init();
    }

    void deinitInternal() override {
        LOGI("Deinit %s", typeName<DerivedT>().c_str());

        events()->post(DeinitEvent());
        deinit();
    }

};

} // flappy
