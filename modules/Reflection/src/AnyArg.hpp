#pragma once

#include "Value.hpp"
#include "ValueRef.hpp"
#include "TypeId.hpp"

namespace flappy {

class Reflection;

class AnyArg {
public:
    AnyArg() = default;
    AnyArg(const AnyArg&) = default; // FIXME: Remove copy constructor
    AnyArg(AnyArg&&) = default;

    template <typename T>
    AnyArg(T&& value, std::enable_if_t<!std::is_convertible_v<T, ValueRef> && !std::is_rvalue_reference_v<T&&>>* = 0)
        : m_valueRef(value)
    {}

    template <typename T>
    AnyArg (T&& value, std::enable_if_t<!std::is_convertible_v<T, ValueRef> && std::is_rvalue_reference_v<T&&>>* = 0)
        : m_tmpValue(std::forward<T>(value))
        , m_valueRef(m_tmpValue)
    {}

    template <typename T>
    AnyArg (T&& value, std::enable_if_t<std::is_convertible_v<T, ValueRef>>* = 0)
        : m_valueRef(value)
    {}

    template <typename T>
    AnyArg (T* value)
        : m_tmpValue(value)
        , m_valueRef(m_tmpValue)
    {}

    AnyArg(void* valuePtr, TypeId typeId)
        : m_valueRef(valuePtr, typeId)
    {}

    template <typename T>
    std::decay_t<T>& as(const Reflection& reflection) const {
        if (getTypeId<T>() != m_valueRef.typeId()) {
            try {
                // FIXME: Implement pointer conversion
                [this](auto reflection) { m_constructedValue = reflection.getType(getTypeId<T>()).constructOnStack(*this); } (reflection);
                return m_constructedValue.as<std::decay_t<T>>();
            } catch (const std::exception& e) {
                throw std::runtime_error(sstr("No convertion to type ", getTypeName(getTypeId<T>())," from type " + getTypeName(m_valueRef.typeId())));
            }
        }
        return m_valueRef.as<T>();
    }

    const ValueRef& valueRef() const { return m_valueRef; }

private:
    Value m_tmpValue;
    ValueRef m_valueRef;
    mutable Value m_constructedValue;
};

} // flappy
