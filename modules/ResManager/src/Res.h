#pragma once

#include <list>
#include <memory>

namespace flappy {

/// @addtogroup ResManager
/// @{

/// Base class for resources.
/// @details Resource manager supports dynamic resource updating.
/// So, if resource is updated, a new instance is pushed to the existing
/// resource instance with pushRes() method.
/// To check if resource updated, check equality of nextRes() and
/// current resource instance.
///
/// Example:
/// @code{.cpp}
/// if (m_res->resUpdated()) {
///     ... // new resource processing
///     m_res = m_res->nextRes()
/// }
/// @endcode
class Res: public std::enable_shared_from_this<Res> {
public:
    Res() = default;
    virtual ~Res() = default;
    Res(const Res&) = delete;
    Res& operator=(const Res&) = delete;
    Res(Res&&) = default;
    Res& operator=(Res&&) = default;

    virtual std::list<std::shared_ptr<Res>> dependencyList() const = 0;

    std::shared_ptr<Res> nextRes() {
        if (m_next != nullptr)
            return m_next;
        else
            return shared_from_this();
    }

    std::shared_ptr<Res> lastRes() {
        if (m_next != nullptr)
            return m_next->lastRes();
        else
            return shared_from_this();
    }

    bool resUpdated() {
        return (m_next != nullptr) && (m_next.get() != this);
    }

    void pushRes(std::shared_ptr<Res> res) {
        if (m_next != nullptr)
            lastRes()->pushRes(res);
        else
            m_next = res;
    }

private:
    std::shared_ptr<Res> m_next;
};

/// @}

} // flappy
