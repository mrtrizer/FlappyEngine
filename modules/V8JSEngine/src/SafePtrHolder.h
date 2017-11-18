#pragma once

#include "CppObjectHolder.h"

namespace flappy
{

template<typename T>
class SafePtrHolder: public CppObjectHolder<T> {
public:
    SafePtrHolder(SafePtr<T> safePtr)
        : CppObjectHolder<T>(HolderType::SAFE)
        , m_safePtr(safePtr)
    {}

    SafePtr<T> safePtr() final { return m_safePtr; }

private:
    SafePtr<T> m_safePtr;
};

} // flappy
