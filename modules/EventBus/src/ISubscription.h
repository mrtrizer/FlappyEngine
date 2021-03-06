#pragma once

#include <type_traits>

//#include <FuncSignature.h>

#include "EventHandle.h"

namespace flappy {

class ISubscription: public std::enable_shared_from_this<ISubscription> {
public:
    virtual ~ISubscription() = default;
    virtual void call(const EventHandle& e) = 0;
};

template <typename EventT>
class Subscription: public ISubscription {
public:
    Subscription(std::function<void(EventT)> func):
        m_func(func)
    {}

    void call(const EventHandle& e) final {
        m_func(*static_cast<const std::decay_t<EventT>*>(e.eventPtr()));
    }

private:
    std::function<void(EventT)> m_func;
};

class SubscriptionAll: public ISubscription {
public:
    SubscriptionAll(std::function<void(const EventHandle&)> func): m_func(func) {}

    void call(const EventHandle& e) final {
        return m_func(e);
    }

private:
    std::function<void(const EventHandle& e)> m_func;
};

} // flappy
