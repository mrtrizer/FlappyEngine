#pragma once

#include <list>
#include <memory>

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

namespace flappy {

class ResBase: public std::enable_shared_from_this<ResBase> {
public:
    ResBase() = default;
    virtual ~ResBase() = default;
    ResBase(const ResBase&) = delete;
    ResBase& operator=(const ResBase&) = delete;
    ResBase(ResBase&&) = default;
    ResBase& operator=(ResBase&&) = default;

    virtual std::list<std::shared_ptr<ResBase>> dependencyList() const { return {}; }

    std::shared_ptr<ResBase> nextResBase();

    std::shared_ptr<ResBase> lastResBase();

    bool resUpdated();

    void pushRes(std::shared_ptr<ResBase> res);

private:
    std::shared_ptr<ResBase> m_next;
};

/// @}

} // flappy
