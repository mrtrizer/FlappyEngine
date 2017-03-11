#ifndef SAFEPTR_H
#define SAFEPTR_H

#include <memory>

namespace  flappy {

template <typename T>
class SafePtr
{
public:
    SafePtr(std::shared_ptr sharedPtr = nullptr): m_weakPtr(sharedPtr) {}
    T* operator->() const { return m_weakPtr.lock().get(); }
    SafePtr operator= (std::shared_ptr sharedPtr) { m_weakPtr = sharedPtr; }

private:
    std::weak_ptr<T> m_weakPtr;
};

} // flappy

#endif // SAFEPTR_H
