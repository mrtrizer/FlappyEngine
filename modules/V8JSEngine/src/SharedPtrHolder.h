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
