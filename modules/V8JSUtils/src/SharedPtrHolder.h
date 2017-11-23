#pragma once

#include "CppObjectHolder.h"

namespace flappy
{

template<typename T>
class SharedPtrHolder: public CppObjectHolder<T> {
public:
    SharedPtrHolder(std::shared_ptr<T> sharedPtr)
        : CppObjectHolder<T>(HolderType::SHARED)
        , m_sharedPtr(sharedPtr)
    {}

    SafePtr<T> safePtr() final { return m_sharedPtr; }
    std::shared_ptr<T> sharedPtr() { return m_sharedPtr; }

private:
    std::shared_ptr<T> m_sharedPtr;
};

} // flappy
