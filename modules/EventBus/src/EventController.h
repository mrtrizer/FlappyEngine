#pragma once

#include <memory>
#include <chrono>
#include "EventBus.h"

#include <SafePtr.h>

namespace flappy {

class ISubscription;

/// Event controller is a wrapper around EventBus.
/// It helps subscribe to events without saving of
/// subscription.
class EventController
{
public:
    EventController();
    virtual ~EventController() = default;
    EventController(const EventController&) = delete;
    EventController& operator=(const EventController&) & = delete;
    EventController(EventController&&) = delete;
    EventController& operator=(EventController&&) & = delete;

    /// Subscribe to concrete event.
    /// Use unsubscribe() method to unsubscribe.
    template <typename FuncT>
    SafePtr<ISubscription> subscribe(FuncT&& func);

    /// Subscribe to all events in event bus. Useful for event forwarding.
    /// Use unsubscribe() method to unsubscribe.
    SafePtr<ISubscription> subscribeAll(std::function<void(const EventHandle& event)> handler);

    /// Unsubscribe of event
    void unsubscribe(SafePtr<ISubscription> subscription);

    /// Post event to all subscribers.
    template<typename EventT>
    void post(EventT&& event);

    void setEventBus(std::shared_ptr<EventBus> eventBus) { m_eventBus = eventBus; }

    std::shared_ptr<EventBus> eventBus() { return m_eventBus; }

private:
    std::shared_ptr<EventBus> m_eventBus;
    std::list<std::shared_ptr<ISubscription>> m_subscriptionVector;
};

template <typename FuncT>
SafePtr<ISubscription> EventController::subscribe(FuncT&& func)
{
    auto subscription = m_eventBus->subscribe(std::forward<FuncT>(func));
    m_subscriptionVector.push_back(subscription);
    return subscription;
}

template<typename EventT>
void EventController::post(EventT&& event) {
#ifdef PROFILE_EVENTS
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
#endif
    m_eventBus->post(std::forward<EventT>(event));
#ifdef PROFILE_EVENTS
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    float duration = duration_cast<microseconds>( t2 - t1 ).count();
    LOG("%s %f", typeName<EventT>().c_str(), duration / 1000.0f);
#endif
}

} // flappy
