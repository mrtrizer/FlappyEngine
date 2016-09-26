#pragma once

namespace flappy {

template <typename Context>
class ClassCounter {
public:
    ClassCounter():
        m_curId(m_count) {
        m_count++;
    }
    unsigned id() {return m_curId;}
    static unsigned count() {return m_count;}
private:
    static unsigned m_count;
    const unsigned m_curId = 0;
};

template <typename Context>
unsigned ClassCounter<Context>::m_count = 0;

template <typename Context, typename Class>
class ClassId {
public:
    /// Returns serial ids for classes in a context.
    /// Base or client classes can be used as a context.
    constexpr static unsigned id() {return m_counter.id();}
private:
    static ClassCounter<Context> m_counter;
};

template <typename Context, typename Class>
ClassCounter<Context> ClassId<Context, Class>::m_counter;

} // flappy
