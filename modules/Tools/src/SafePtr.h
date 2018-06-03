#pragma once

#include <memory>
#include <stdexcept>
#include <Tools.h>

namespace  flappy {

class SafePtrBase
{
public:
    virtual ~SafePtrBase() = default;
};

template <typename T>
class SafePtr: public SafePtrBase
{
public:
    SafePtr(std::nullptr_t) :
        m_weakPtr()
    {}

    SafePtr(std::shared_ptr<T> sharedPtr = nullptr):
        m_weakPtr(sharedPtr)
    {}

    SafePtr(std::weak_ptr<T> weakPtr):
        m_weakPtr(weakPtr)
    {}

    template <typename SafePatrT>
    SafePtr(const SafePtr<SafePatrT>& sharedPtr):
        m_weakPtr(std::static_pointer_cast<T>(sharedPtr.m_weakPtr.lock()))
    {}

    T* operator->() const
    {
        auto sharedPtr = m_weakPtr.lock();
        if (sharedPtr)
            return sharedPtr.get();
        else {
            Tools::printStackTrace();
            throw std::runtime_error("Pointer is expired");
        }
    }

    SafePtr& operator= (std::shared_ptr<T> sharedPtr)
    {
        m_weakPtr = sharedPtr;
        return *this;
    }

    // TODO: Add test
    bool operator== (const SafePtr<T>& safePtr) const {
        return m_weakPtr.lock() == safePtr.m_weakPtr.lock();
    }

    bool operator== (const std::shared_ptr<T>& sharedPtr) const {
        return m_weakPtr.lock() == sharedPtr;
    }

    bool operator== (const T* rawPtr) const {
        return m_weakPtr.lock().get() == rawPtr;
    }

    // TODO: Add test
    bool operator!= (const SafePtr<T>& safePtr) const {
        return !(m_weakPtr.lock() == safePtr.m_weakPtr.lock());
    }

    // TODO: Add test
    bool operator!= (const std::shared_ptr<T>& sharedPtr) const {
        return !(m_weakPtr.lock() == sharedPtr);
    }

    // TODO: Add test
    bool operator!= (const T* rawPtr) const {
        return !(m_weakPtr.lock().get() == rawPtr);
    }

    operator bool() const
    {
        return !m_weakPtr.expired();
    }

    // TODO: Move m_weakPtr to private
    std::weak_ptr<T> m_weakPtr;
};

} // flappy
