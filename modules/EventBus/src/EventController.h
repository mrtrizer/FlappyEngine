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

    template <typename EventT, typename FuncT>
    std::shared_ptr<ISubscription> subscribe(FuncT&& func) {
        auto subscription = m_eventBus->subscribe<EventT>(std::forward<FuncT>(func));
        return subscription;
    }

    void unsubscribe(std::shared_ptr<ISubscription> subscription);
    template<typename EventT>
    void post(EventT&& event) {
        m_eventBus->post(std::forward<EventT>(event));
    }

    std::shared_ptr<EventBus> eventBus() {return m_eventBus;}

private:
    std::shared_ptr<EventBus> m_eventBus;
};

} // flappy
