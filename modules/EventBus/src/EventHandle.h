#pragma once

#include <memory>
#include <type_traits>

#include <ClassId.h>
#include <TypeTraits.h>

namespace flappy {

class IEvent {
public:
    IEvent() = default;
    IEvent(const IEvent&) = default;
    IEvent& operator=(const IEvent&) & = default;
    IEvent(IEvent&&) = default;
    IEvent& operator=(IEvent&&) & = default;
    virtual ~IEvent() = default;
};

class EventBus;

class EventHandle {
public:
    template <typename EventT>
    EventHandle(EventT&& event):
        m_id(ClassId<EventHandle, EventT>::id()),
        m_eventStructPtr(new std::decay_t<EventT>(std::forward<EventT>(event)))
    {
        static_assert(isBaseOf<IEvent, EventT>(), "Event must be a descendant of IEvent");
    }

    EventHandle(const EventHandle&) = delete;
    EventHandle& operator=(const EventHandle&) & = delete;
    EventHandle(EventHandle&& event) = default;
    EventHandle& operator=(EventHandle&&) & = default;

    IEvent* eventPtr() const { return m_eventStructPtr.get(); }
    unsigned id() const { return m_id; }

private:
    unsigned m_id;
    std::unique_ptr<IEvent> m_eventStructPtr;
};

} // flappy
