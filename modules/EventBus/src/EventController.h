#pragma once

#include <memory>
#include "EventBus.h"


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
    std::shared_ptr<ISubscription> subscribeIn(FuncT&& func)
    {
        auto subscription = m_eventBus->subscribeIn(std::forward<FuncT>(func));
        m_subscriptionVector.push_back(subscription);
        return subscription;
    }

    std::shared_ptr<ISubscription> subscribeInAll(std::function<FlowStatus(const EventHandle& event)> handler)
    {
        auto subscription = m_eventBus->subscribeInAll(handler);
        m_subscriptionVector.push_back(subscription);
        return subscription;
    }

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribeOut(FuncT&& func)
    {
        auto subscription = m_eventBus->subscribeOut(std::forward<FuncT>(func));
        m_subscriptionVector.push_back(subscription);
        return subscription;
    }

    void unsubscribe(std::shared_ptr<ISubscription> subscription);

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
