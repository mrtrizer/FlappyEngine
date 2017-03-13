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
    EventBus()
    {}

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribeIn(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return subscribeInList(std::forward<FuncT>(func), m_inSubscriptions.get<EventT>());
    }

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribeOut(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return subscribeInList(std::forward<FuncT>(func), m_outSubscriptions.get<EventT>());
    }

    std::shared_ptr<ISubscription> subscribeInAll(std::function<FlowStatus(const EventHandle& event)> handler) {
        auto subscription = std::make_shared<SubscriptionAll>(handler);
        m_abstractSubscriptions.push_back(subscription);
        return subscription;
    }

    FlowStatus post(const EventHandle& event) {
        auto id = event.id();
        if (postInList(event, m_inSubscriptions.getById(id)) == FlowStatus::BREAK) {
            postInList(event, m_outSubscriptions.getById(id));
            return FlowStatus::CONTINUE;
        }
        if (postInList(event, m_abstractSubscriptions) == FlowStatus::CONTINUE)
            return postInList(event, m_outSubscriptions.getById(id));
        else
            return FlowStatus::BREAK;
    }

private:
    TypeMap<EventHandle, std::list<std::weak_ptr<ISubscription>>> m_inSubscriptions;
    std::list<std::weak_ptr<ISubscription>> m_abstractSubscriptions;
    TypeMap<EventHandle, std::list<std::weak_ptr<ISubscription>>> m_outSubscriptions;

    FlowStatus postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription> > &subscriptions);

    template <typename FuncT, typename CompT>
    using IsSameResult = std::is_same<typename FuncSignature<FuncT>::result_type, CompT>;

    template <typename FuncT>
    typename std::enable_if<IsSameResult<FuncT, void>::value, std::shared_ptr<ISubscription> >::type
    createSubscription(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return std::make_shared<Subscription<EventT>>(std::forward<FuncT>(func));
    }

    template <typename FuncT>
    typename std::enable_if<IsSameResult<FuncT, FlowStatus>::value, std::shared_ptr<ISubscription> >::type
    createSubscription(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return std::make_shared<SubscriptionControl<EventT>>(std::forward<FuncT>(func));
    }

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    template<typename FuncT>
    std::shared_ptr<ISubscription> subscribeInList(FuncT&& func, std::list<std::weak_ptr<ISubscription>>& subscriptions)
    {
        auto subscription = createSubscription(func);
        subscriptions.push_back(subscription);
        return subscription;
    }
};

} // flappy
