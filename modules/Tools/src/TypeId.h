#pragma once

#include <string>
#include <unordered_map>
#include <Tools.h>

#include "TypeNames.h"

namespace flappy {

template <typename ContextT>
class TypeCounter {
public:
    TypeCounter(std::string name):
        m_curId(m_count) {
        m_count++;
        TypeNames<ContextT>::instance().setName(m_curId, name);
    }
    unsigned id() { return m_curId; }
    static unsigned count() { return m_count; }
private:
    static unsigned m_count;
    const unsigned m_curId;
};

template <typename ContextT>
unsigned TypeCounter<ContextT>::m_count = 0;

template <typename ContextT>
class TypeId {
public:
    explicit TypeId(unsigned id = -1)
        : m_id(id)
        , m_name(typeName<ContextT>())
    {}
    explicit TypeId(std::string name)
        : m_id(TypeNames<ContextT>::instance().getIndexByName(name))
        , m_name(name)
    {}
    TypeId(const TypeId&) = default;
    TypeId(TypeId&&) = default;
    TypeId& operator = (const TypeId&) = default;
    bool operator>(const TypeId& other) const { return other.toUnsigned() > toUnsigned(); }
    bool operator<(const TypeId& other) const { return other.toUnsigned() < toUnsigned(); }
    bool operator==(const TypeId& other) const { return other.toUnsigned() == toUnsigned(); }
    bool operator!=(const TypeId& other) const { return other.toUnsigned() != toUnsigned(); }
    unsigned toUnsigned() const { return m_id; }
    bool isValid() const { return m_id != -1; }
    std::string name() { return TypeNames<ContextT>::instance().getName(m_id); }
private:
    unsigned m_id;
    std::string m_name;
};

template <typename ContextT, typename TypeT>
class GetTypeId_ {
public:
    /// Returns serial ids for classes in a context.
    /// Base or client classes can be used as a context.
    static TypeId<ContextT> value() {return TypeId<ContextT>(m_counter.id());}
private:
    static TypeCounter<ContextT> m_counter;
};

template <typename ContextT, typename TypeT>
TypeCounter<ContextT> GetTypeId_<ContextT, TypeT>::m_counter(typeName<TypeT>());

template <typename ContextT, typename ClassT>
using GetTypeId = GetTypeId_<std::decay_t<ContextT>, std::decay_t<ClassT>>;

} // flappy