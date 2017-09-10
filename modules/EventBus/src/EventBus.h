#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

#include <TypeMap.h>
#include <FuncSignature.h>

#include "ISubscription.h"

namespace flappy {


class EventBus {
public:
    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribe(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return subscribeList(std::forward<FuncT>(func), m_inSubscriptions.get<EventT>());
    }

    std::shared_ptr<ISubscription> subscribeAll(std::function<void(const EventHandle& event)> handler) {
        auto subscription = std::make_shared<SubscriptionAll>(handler);
        m_abstractSubscriptions.push_back(subscription);
        return subscription;
    }

    void post(const EventHandle& event) {
        auto id = event.id();
        postInList(event, m_inSubscriptions.getById(id));
        postInList(event, m_abstractSubscriptions);
    }

private:
    TypeMap<EventHandle, std::list<std::weak_ptr<ISubscription>>> m_inSubscriptions;
    std::list<std::weak_ptr<ISubscription>> m_abstractSubscriptions;

    void postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription> > &subscriptions);

    template <typename FuncT, typename CompT>
    using IsSameResult = std::is_same<typename FuncSignature<FuncT>::result_type, CompT>;

    template <typename FuncT>
    std::shared_ptr<ISubscription> createSubscription(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return std::make_shared<Subscription<EventT>>(std::forward<FuncT>(func));
    }

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    template<typename FuncT>
    std::shared_ptr<ISubscription> subscribeList(FuncT&& func, std::list<std::weak_ptr<ISubscription>>& subscriptions)
    {
        auto subscription = createSubscription(func);
        subscriptions.push_back(subscription);
        return subscription;
    }

};

} // flappy
