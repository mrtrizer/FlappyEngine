#pragma once

#include <memory>

#include "ValueRef.hpp"
#include "TypeId.hpp"

namespace flappy {

// FIXME: Value should be deeply copied. Automatic operator detection needed.
class Value : public ValueRef {
public:
    Value() = default;
    Value(const Value&) = default;
    Value& operator=(const Value& other) = default;
    Value(Value&&) = default;
    Value& operator=(Value&&) = default;

    template <typename T, typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T&& value)
        : m_value(std::make_shared<std::decay_t<T>>(std::forward<T>(value)))
    {
        setValuePtr(m_value.get(), getTypeId<T>());
    }

    ValueRef deref() {
        if (!typeId().isPointer())
            throw FlappyException("Type is not a pointer");
        return ValueRef(*std::static_pointer_cast<void*>(m_value), TypeId(typeId(), false, typeId().isConst()));
    }

private:
    std::shared_ptr<void> m_value;
};

} // flappy
