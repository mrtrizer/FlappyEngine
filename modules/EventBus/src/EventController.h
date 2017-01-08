#pragma once

#include <memory>
#include "EventBus.h"


namespace flappy {

class ISubscription;

class EventController
{
public:
    EventController(std::shared_ptr<EventBus> parentEventBus = nullptr);
    virtual ~EventController() = default;
    EventController(const EventController&) = delete;
    EventController& operator=(const EventController&) & = delete;
    EventController(EventController&&) = delete;
    EventController& operator=(EventController&&) & = delete;

    template <typename FuncT>
    void subscribe(FuncT&& func) {
        m_subscriptionVector.push_back(m_eventBus->subscribe(std::forward<FuncT>(func)));
    }

    template <typename FuncT, typename SourceT>
    void subscribe(SourceT source, FuncT&& func) {
        m_subscriptionVector.push_back(source->events()->eventBus()->subscribe(std::forward<FuncT>(func)));
    }

    void unsubscribe(std::shared_ptr<ISubscription> subscription);

    template<typename EventT>
    void post(EventT&& event) {
        m_eventBus->post(std::forward<EventT>(event));
    }

    std::shared_ptr<EventBus> eventBus() {return m_eventBus;}

private:
    std::shared_ptr<EventBus> m_eventBus;
    std::vector<std::shared_ptr<ISubscription>> m_subscriptionVector;
};

} // flappy
