#pragma once

#include <unordered_map>
#include <vector>

#include "TypeId.hpp"
#include "Function.hpp"
#include "Constructor.hpp"

namespace flappy {

class Field {
public:
    template <typename TypeT, typename FieldT>
    Field(const Reflection& reflection, FieldT TypeT::*fieldPtr)
        : m_typeId(getTypeId<FieldT>())
        , m_setter([fieldPtr, &reflection](ValueRef& obj, const AnyArg& anyArg)
            { reinterpret_cast<TypeT*>(obj.voidPointer())->*fieldPtr = anyArg.as<FieldT>(reflection); })
        , m_getter([fieldPtr](const ValueRef& obj)
            { return Value(reinterpret_cast<TypeT*>(obj.voidPointer())->*fieldPtr); })
    {}

    const TypeId& typeId() const { return m_typeId; }

    void setValue(ValueRef& obj, const AnyArg& anyValue) const { m_setter(obj, anyValue); }

    Value getValue(const ValueRef& obj) const { return m_getter(obj); }

private:
    TypeId m_typeId;
    std::function<void(ValueRef&, const AnyArg&)> m_setter;
    std::function<Value(const ValueRef&)> m_getter;
};

} // flappy
