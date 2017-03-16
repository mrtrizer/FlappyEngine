#pragma once

#include <type_traits>

#include <FuncSignature.h>

#include "EventHandle.h"

namespace flappy {

enum class FlowStatus {
    BREAK,
    CONTINUE
};

class ISubscription {
public:
    virtual ~ISubscription() = default;
    virtual FlowStatus call(const EventHandle& e) = 0;
};

template <typename EventT>
class Subscription: public ISubscription {
public:
    Subscription(std::function<void(EventT)> func): m_func(func)
    {}

    FlowStatus call(const EventHandle& e) override {
        m_func(*static_cast<const std::decay_t<EventT>*>(e.eventPtr()));
        return FlowStatus::CONTINUE;
    }

private:
    std::function<void(EventT)> m_func;
};

template <typename EventT>
class SubscriptionControl: public ISubscription {
public:
    SubscriptionControl(std::function<FlowStatus(EventT)> func): m_func(func)
    {}

    FlowStatus call(const EventHandle& e) override {
        return m_func(*static_cast<const std::decay_t<EventT>*>(e.eventPtr()));
    }

private:
    std::function<FlowStatus(EventT)> m_func;
};

class SubscriptionAll: public ISubscription {
public:
    SubscriptionAll(std::function<FlowStatus(const EventHandle&)> func): m_func(func) {}

    FlowStatus call(const EventHandle& e) override {
        return m_func(e);
    }

private:
    std::function<FlowStatus(const EventHandle& e)> m_func;
};

} // flappy
