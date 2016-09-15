#pragma once

namespace flappy {

/// Gives index for every class, derived from BaseClass
template <typename BaseClass>
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

template <typename BaseClass>
unsigned ClassCounter<BaseClass>::m_count = 0;

template <typename BaseClass, typename DerivedClass>
class ClassId {
public:
    constexpr static unsigned id() {return m_counter.id();}
private:
    static ClassCounter<BaseClass> m_counter;
};

template <typename BaseClass, typename DerivedClass>
ClassCounter<BaseClass> ClassId<BaseClass, DerivedClass>::m_counter;

} // flappy
