#pragma once

namespace flappy {

template <typename ContextT>
class TypeCounter {
public:
    TypeCounter():
        m_curId(m_count) {
        m_count++;
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
private:
    unsigned m_id;
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

template <typename ContextT, typename ClassT>
TypeCounter<ContextT> GetTypeId_<ContextT, ClassT>::m_counter;

template <typename ContextT, typename ClassT>
using GetTypeId = GetTypeId_<std::decay_t<ContextT>, std::decay_t<ClassT>>;

} // flappy
