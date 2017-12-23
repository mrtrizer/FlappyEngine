#pragma once

#include <vector>
#include <stdexcept>

#include "TypeId.h"

namespace  flappy
{

template <typename ContextT, typename ElementT>
class TypeMap
{
public:
    TypeMap() :
        m_elements(TypeCounter<ContextT>::count())
    {}

    template<typename T>
    struct Pair {
        TypeId<ContextT> first;
        T& second;
    };

    template<typename IteratorT>
    struct Iterator {
        Iterator(IteratorT element, TypeId<ContextT> typeId)
            : m_index(typeId)
            , m_vectorIterator(element)
        {}

        Iterator& operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator iter(m_vectorIterator, m_index);
            increment();
            return iter;
        }

        bool operator==(const Iterator& other) {
            return other.m_index == m_index;
        }

        bool operator!=(const Iterator& other) {
            return other.m_index != m_index;
        }

        Pair<typename IteratorT::reference> operator*() {
            return { m_index, *m_vectorIterator };
        }

    private:
        TypeId<ContextT> m_index;
        IteratorT m_vectorIterator;

        void increment() {
            m_index = TypeId<ContextT>(m_index.toUnsigned() + 1);
            m_vectorIterator++;
        }
    };

    Iterator<typename std::vector<ElementT>::iterator> begin()
    {
        std::vector<int>::const_iterator i;
        return {m_elements.begin(), TypeId<ContextT>(0)};
    }

    Iterator<typename std::vector<ElementT>::iterator> end()
    {
        return {m_elements.end(), TypeId<ContextT>(m_elements.size())};
    }

    Iterator<typename std::vector<ElementT>::const_iterator> begin() const
    {
        return {m_elements.begin(), TypeId<ContextT>(0)};
    }

    Iterator<typename std::vector<ElementT>::const_iterator> end() const
    {
        return {m_elements.end(), TypeId<ContextT>(m_elements.size())};
    }

    // TODO: Add test
    int size() {
        return int(m_elements.size());
    }

    ElementT& getByName(std::string name)
    {
        unsigned index = TypeNames<ContextT>::instance().getIndexByName(name);
        return m_elements.at(index);
    }

    const ElementT& getByName(std::string name) const
    {
        unsigned index = TypeNames<ContextT>::instance().getIndexByName(name);
        return m_elements.at(index);
    }

    ElementT& getById(TypeId<ContextT> id)
    {
        return m_elements.at(id.toUnsigned());
    }

    const ElementT& getById(TypeId<ContextT> id) const
    {
        return m_elements.at(id.toUnsigned());
    }

    template <typename T>
    ElementT& get()
    {
        return getById(GetTypeId<ContextT, T>::value());
    }

    template <typename T>
    const ElementT& get() const
    {
        return getById(GetTypeId<ContextT, T>::value());
    }

    // TODO: Add test
    template <typename ValueT>
    void setById(TypeId<ContextT> id, ValueT&& element)
    {
        m_elements[id.toUnsigned()] = std::forward<ValueT>(element);
    }

    template <typename T, typename ValueT>
    void set(ValueT&& element)
    {
        setById(GetTypeId<ContextT, T>::value(), std::forward<ValueT>(element));
    }

private:
    std::vector<ElementT> m_elements;
};

} // flappy
