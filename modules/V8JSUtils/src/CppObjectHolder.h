#pragma once

#include <memory>

#include <SafePtr.h>

namespace flappy
{

class CppObjectHolderBase {
public:
    virtual ~CppObjectHolderBase() = default;
};

enum class HolderType {
    SHARED,
    SAFE
};

template<typename T>
class CppObjectHolder: public CppObjectHolderBase {
public:
    HolderType holderType() { return m_holderType; }
    virtual SafePtr<T> safePtr() = 0;

protected:
    CppObjectHolder(HolderType holderType)
        : m_holderType(holderType)
    {}

private:
    HolderType m_holderType;
};

} // flappy
