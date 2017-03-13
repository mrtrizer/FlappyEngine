#ifndef SAFEPTR_H
#define SAFEPTR_H

#include <memory>

namespace  flappy {

template <typename T>
class SafePtr
{
public:
    SafePtr(std::shared_ptr<T> sharedPtr = nullptr):
        m_weakPtr(sharedPtr)
    {}

    SafePtr(std::weak_ptr<T> weakPtr):
        m_weakPtr(weakPtr)
    {}

    template <typename SafePatrT>
    SafePtr(SafePtr<SafePatrT> sharedPtr = nullptr):
        m_weakPtr(std::static_pointer_cast<T>(sharedPtr.m_weakPtr.lock()))
    {}

    T* operator->() const
    {
        auto sharedPtr = m_weakPtr.lock();
        if (sharedPtr)
            return sharedPtr.get();
        else
            throw std::runtime_error("Pointer is expired");
    }

    SafePtr& operator= (std::shared_ptr<T> sharedPtr)
    {
        m_weakPtr = sharedPtr;
        return *this;
    }

    bool operator== (std::shared_ptr<T> sharedPtr) {
        return m_weakPtr.lock() == sharedPtr;
    }

    bool operator== (T* rawPtr) {
        return m_weakPtr.lock().get() == rawPtr;
    }

    operator bool()
    {
        return !m_weakPtr.expired();
    }

    std::weak_ptr<T> m_weakPtr;
};

} // flappy

#endif // SAFEPTR_H
