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

private:
    std::weak_ptr<T> m_weakPtr;
};

} // flappy

#endif // SAFEPTR_H
