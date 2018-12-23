#pragma once

#include <memory>

#include <TypeId.hpp>

#include "IEvent.h"

namespace flappy {

class EventBus;

class EventHandle {
public:
    template <typename EventT>
    EventHandle(EventT&& event):
        m_id(getTypeId<EventT>()),
        m_eventStructPtr(new std::decay_t<EventT>(std::forward<EventT>(event)))
    {
        static_assert(std::is_base_of<IEvent, std::decay_t<EventT>>::value, "Event must be a descendant of IEvent");
    }

    EventHandle(const EventHandle&) = delete;
    EventHandle& operator=(const EventHandle&) & = delete;
    EventHandle(EventHandle&& event) = default;
    EventHandle& operator=(EventHandle&&) & = default;

    IEvent* eventPtr() const { return m_eventStructPtr.get(); }
    TypeId id() const { return m_id; }

private:
    TypeId m_id;
    std::unique_ptr<IEvent> m_eventStructPtr;
};

} // flappy
