#pragma once

#include "TypeId.hpp"

namespace flappy {

class Value;

class ValueRef {
public:
    ValueRef() = default;
    ValueRef(const ValueRef&) = default;
    ValueRef& operator=(const ValueRef&) = default;
    ValueRef(ValueRef&&) = default;
    ValueRef& operator=(ValueRef&&) = default;

    TypeId typeId() const { return m_typeId; }

    void* voidPointer() const { return const_cast<void*>(m_valuePtr); }

    ValueRef(void* pointer, TypeId typeId)
        : m_valuePtr(pointer)
        , m_typeId(typeId)
    {}

    template <typename T>
    ValueRef(const T& value, std::enable_if_t<!std::is_convertible<T, Value>::value>* = 0)
        : m_valuePtr(&value)
        , m_typeId(getTypeId<T>())
    {}

    template <typename T>
    ValueRef(const T& value, std::enable_if_t<std::is_convertible<T, Value>::value>* = 0)
        : m_valuePtr(value.voidPointer())
        , m_typeId(value.typeId())
    {}

    template <typename T>
    T& as() const {
        if (getTypeId<T>() != m_typeId)
            throw std::runtime_error(sstr("Value of wrong type! Expects: ", getTypeName(getTypeId<T>()), " Passed: ", getTypeName(m_typeId)));
        return *static_cast<std::remove_reference_t<T>*>(const_cast<void*>(m_valuePtr));
    }

protected:
    void setValuePtr(void* valuePtr, TypeId typeId) {
        m_valuePtr = valuePtr;
        m_typeId = typeId;
    }

private:
    const void* m_valuePtr = nullptr;
    TypeId m_typeId;
};

} // flappy
