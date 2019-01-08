#pragma once

#include <optional>

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
        , m_valueRef(*m_tmpValue)
    {}

    template <typename T>
    AnyArg (T&& value, std::enable_if_t<std::is_convertible_v<T, ValueRef>>* = 0)
        : m_valueRef(value)
    {}

    template <typename T>
    AnyArg (T* value)
        : m_tmpValue(value)
        , m_valueRef(*m_tmpValue)
    {}

    AnyArg(void* valuePtr, TypeId typeId)
        : m_valueRef(valuePtr, typeId)
    {}

    template <typename T>
    std::decay_t<T>& as(const Reflection& reflection) const {
        auto typeId = getTypeId<std::decay_t<T>>();
        if (typeId != m_valueRef.typeId() && !(typeId.isPointer() && m_valueRef.typeId().isPointer())) {
            try {
                [this, &typeId](auto reflection) {
                    m_constructedValue = reflection.getType(typeId).constructOnStack(*this);
                } (reflection);
                return m_constructedValue->as<std::decay_t<T>>();
            } catch (const std::exception& e) {
                throw std::runtime_error(sstr(e.what(),
                            "\nNo convertion to type ", getTypeName(typeId),
                            " from type " + getTypeName(m_valueRef.typeId())));
            }
        }
        return m_valueRef.as<std::decay_t<T>>();
    }

    const ValueRef& valueRef() const { return m_valueRef; }

private:
    std::optional<Value> m_tmpValue;
    ValueRef m_valueRef;
    mutable std::optional<Value> m_constructedValue;
};

} // flappy
