#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>
#include <vector>

#include <core/classid.h>

namespace flappy {

// http://stackoverflow.com/questions/9065081/how-do-i-get-the-argument-types-of-a-function-pointer-in-a-variadic-template-cla
template<typename T>
struct function_traits;

template<typename R, typename ...Args>
struct function_traits<std::function<R(Args...)>>
{
    static const size_t nargs = sizeof...(Args);

    typedef R result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

// http://stackoverflow.com/questions/13358672/how-to-convert-a-lambda-to-an-stdfunction-using-templates
template<typename T> struct remove_class { };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...)> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

template<typename T>
struct get_signature_impl { using type = typename remove_class<
    decltype(&std::remove_reference<T>::type::operator())>::type; };
template<typename R, typename... A>
struct get_signature_impl<R(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
template<typename T> using get_signature = typename get_signature_impl<T>::type;

template<typename T>
using FuncSignature = function_traits<std::function<get_signature<T>>>;


class ISubscription {
public:
    virtual ~ISubscription() = default;
    virtual void exec() = 0;
};

enum class SyncType {
    DIRECT,
    QUEUED
};

template <typename EventT>
class Subscription: public ISubscription {
public:
    Subscription(std::function<void(EventT)> func, SyncType sync): m_func(func), m_sync(sync) {}

    void call(const EventT& e) {
        m_func(e);
    }

    void add(const EventT& e) {
        if (m_sync == SyncType::DIRECT) {
            call(e);
        } else {
            m_eventQueue.push_back(e);
        }
    }

    void exec() override {
        for (auto& event: m_eventQueue)
            call(event);
        m_eventQueue.clear();
    }

private:
    std::function<void(EventT)> m_func;
    SyncType m_sync;
    std::list<EventT> m_eventQueue;
};

class EventSystem {
public:

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    /// @param synchronization type QUEUED/DIRECT
    template<typename FuncT>
    void subscribe(FuncT func, SyncType sync = SyncType::QUEUED) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        const unsigned id = ClassId<EventSystem, EventT>::id();
        const std::thread::id threadId = std::this_thread::get_id();
        std::lock_guard<std::mutex> lock(syncMutex);
        if (m_subscriptions[threadId].size() <= id)
            m_subscriptions[threadId].resize(id + 1);
        m_subscriptions[threadId][id].push_back(std::make_shared<Subscription<EventT>>(func, sync));
    }

    /// Emit event to all listeners
    template<typename EventT>
    void post(EventT event) {
        const unsigned id = ClassId<EventSystem, EventT>::id();
        if (m_subscriptions.size() <= id)
            return;

        std::lock_guard<std::mutex> lock(syncMutex);
        for (auto& thread: m_subscriptions)
            for (auto& subscription: thread.second[id])
                std::static_pointer_cast<Subscription<EventT>>(subscription)->add(event);
    }

    /// Process queued events for current thread
    void exec();

private:
    std::unordered_map<std::thread::id, std::vector<std::list<std::shared_ptr<ISubscription>>>> m_subscriptions;
    std::mutex syncMutex;
};

} // flappy
