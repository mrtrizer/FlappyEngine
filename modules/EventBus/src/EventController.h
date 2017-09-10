#pragma once

#include <memory>
#include "EventBus.h"

#include <SafePtr.h>

namespace flappy {

class ISubscription;

class EventController
{
public:
    EventController();
    virtual ~EventController() = default;
    EventController(const EventController&) = delete;
    EventController& operator=(const EventController&) & = delete;
    EventController(EventController&&) = delete;
    EventController& operator=(EventController&&) & = delete;

    template <typename FuncT>
    SafePtr<ISubscription> subscribe(FuncT&& func)
    {
        auto subscription = m_eventBus->subscribe(std::forward<FuncT>(func));
        m_subscriptionVector.push_back(subscription);
        return subscription;
    }

    SafePtr<ISubscription> subscribeAll(std::function<void(const EventHandle& event)> handler)
    {
        auto subscription = m_eventBus->subscribeAll(handler);
        m_subscriptionVector.push_back(subscription);
        return subscription;
    }

    void unsubscribe(SafePtr<ISubscription> subscription);

    template<typename EventT>
    void post(EventT&& event) {
        m_eventBus->post(std::forward<EventT>(event));
    }

    void setEventBus(std::shared_ptr<EventBus> eventBus) { m_eventBus = eventBus; }
    std::shared_ptr<EventBus> eventBus() { return m_eventBus; }

private:
    std::shared_ptr<EventBus> m_eventBus;
    std::list<std::shared_ptr<ISubscription>> m_subscriptionVector;
};

} // flappy
