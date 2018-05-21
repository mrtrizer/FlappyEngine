#pragma once

#include <string>
#include <unordered_map>
#include <Tools.h>
#include <type_traits>

#include "TypeNames.h"

#define SHARED_TYPE_ID

namespace flappy {

#ifndef SHARED_TYPE_ID

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
#endif
template <typename ContextT>
class TypeId {
public:
    TypeId(const TypeId&) = default;
    TypeId(TypeId&&) = default;
    TypeId& operator = (const TypeId&) = default;
#ifndef SHARED_TYPE_ID
    explicit TypeId(unsigned id = -1)
        : m_id(id)
        , m_name(TypeNames<ContextT>::instance().getName(id))
    {}
    explicit TypeId(std::string name)
        : m_id(TypeNames<ContextT>::instance().getIndexByName(name))
        , m_name(name)
    {}
    bool operator>(const TypeId& other) const { return other.toUnsigned() > toUnsigned(); }
    bool operator<(const TypeId& other) const { return other.toUnsigned() < toUnsigned(); }
    bool operator==(const TypeId& other) const { return other.toUnsigned() == toUnsigned(); }
    bool operator!=(const TypeId& other) const { return other.toUnsigned() != toUnsigned(); }
    unsigned toUnsigned() const { return m_id; }
    bool isValid() const { return m_id != -1; }
#else
    explicit TypeId(std::string name = "")
        : m_name(name)
    {}
    bool operator>(const TypeId& other) const { return other.name() > name(); }
    bool operator<(const TypeId& other) const { return other.name() < name(); }
    bool operator==(const TypeId& other) const { return other.name() == name(); }
    bool operator!=(const TypeId& other) const { return other.name() != name(); }
    bool isValid() const { return !m_name.empty(); }
#endif
    const std::string& name() const { return m_name; }
private:
#ifndef SHARED_TYPE_ID
    unsigned m_id;
#endif
    std::string m_name;
};

template <typename ContextT, typename TypeT>
class GetTypeId_ {
public:
    /// Returns serial ids for classes in a context.
    /// Base or client classes can be used as a context.
#ifndef SHARED_TYPE_ID
    static TypeId<ContextT> value() {return TypeId<ContextT>(m_counter.id());}
private:
    static TypeCounter<ContextT> m_counter;
#else
    static TypeId<ContextT> value() {return TypeId<ContextT>(typeName<TypeT>());}
#endif
};

#ifndef SHARED_TYPE_ID
template <typename ContextT, typename TypeT>
TypeCounter<ContextT> GetTypeId_<ContextT, TypeT>::m_counter(typeName<TypeT>());
#endif

template <typename ContextT, typename ClassT>
using GetTypeId = GetTypeId_<std::decay_t<ContextT>, std::decay_t<ClassT>>;

} // flappy
