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
    Component()
        : m_typeId(GetTypeId<ComponentBase, DerivedT>::value())
    {}

    TypeId<ComponentBase> componentId() const final {
        return m_typeId;
    }

    static TypeId<ComponentBase> id() {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

    SafePtr<DerivedT> selfPointer() { return std::static_pointer_cast<DerivedT>(shared_from_this()); }
    const SafePtr<DerivedT> selfPointer() const { return std::static_pointer_cast<DerivedT>(shared_from_this()); }

private:
    void initInternal() final {
        LOGI("Init %s", typeName<DerivedT>().c_str());

        events()->post(InitEvent());
    }

    void deinitInternal() final {
        LOGI("Deinit start %s", typeName<DerivedT>().c_str());

        events()->post(DeinitEvent());

        LOGI("Deinit end %s", typeName<DerivedT>().c_str());
    }

    TypeId<ComponentBase> m_typeId;
};

} // flappy
