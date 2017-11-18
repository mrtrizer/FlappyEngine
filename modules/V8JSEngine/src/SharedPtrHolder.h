#pragma once

#include <memory>

namespace flappy
{

class CppObjectHolder {
public:
    virtual ~CppObjectHolder() = default;
};

template<typename T>
class SharedPtrHolder: public CppObjectHolder {
public:
    SharedPtrHolder(std::shared_ptr<T> sharedPtr)
        : m_sharedPtr(sharedPtr)
    {}

    std::shared_ptr<T> sharedPtr() { return m_sharedPtr; }

private:
    std::shared_ptr<T> m_sharedPtr;
};

} // flappy
