#pragma once

#include <memory>

#include <ClassId.h>

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
        m_eventStructPtr(new EventT(std::forward<EventT>(event)))
    {}

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
