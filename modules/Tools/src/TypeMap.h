#pragma once

#include <vector>
#include <stdexcept>
#include <map>

#include "TypeId.h"

namespace  flappy
{
template <typename ContextT, typename ElementT>
class TypeMap
{
public:
#ifndef SHARED_TYPE_ID
    using TypeMapListT = std::vector<ElementT>;

    TypeMap()
        : m_elements(TypeCounter<ContextT>::count())
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
            m_vectorIterator++;
            m_index = TypeId<ContextT>(m_index.toUnsigned() + 1);
        }
    };


    Iterator<typename TypeMapListT::iterator> begin()
    {
        return {m_elements.begin(), TypeId<ContextT>(0)};
    }

    Iterator<typename TypeMapListT::iterator> end()
    {
        return {m_elements.end(), TypeId<ContextT>(m_elements.size())};
    }

    Iterator<typename TypeMapListT::const_iterator> begin() const
    {
        return {m_elements.begin(), TypeId<ContextT>(0)};
    }

    Iterator<typename TypeMapListT::const_iterator> end() const
    {
        return {m_elements.end(), TypeId<ContextT>(m_elements.size())};
    }

#else
    using TypeMapListT = std::map<TypeId<ContextT>, ElementT>;

    typename TypeMapListT::iterator begin()
    {
        return m_elements.begin();
    }

    typename TypeMapListT::iterator end()
    {
        return m_elements.end();
    }

    typename TypeMapListT::const_iterator begin() const
    {
        return m_elements.begin();
    }

    typename TypeMapListT::const_iterator end() const
    {
        return m_elements.end();
    }
#endif

    // TODO: Add test
    int size() {
        return int(m_elements.size());
    }

    ElementT& getByName(std::string name)
    {
#ifndef SHARED_TYPE_ID
        unsigned index = TypeNames<ContextT>::instance().getIndexByName(name);
        return m_elements.at(index);
#else
        return getById(TypeId<ContextT>(name));
#endif
    }

    const ElementT& getByName(std::string name) const
    {
#ifndef SHARED_TYPE_ID
        unsigned index = TypeNames<ContextT>::instance().getIndexByName(name);
        return m_elements.at(index);
#else
        return getById(TypeId<ContextT>(name));
#endif
    }

    ElementT& getById(TypeId<ContextT> id)
    {
#ifndef SHARED_TYPE_ID
        return m_elements.at(id.toUnsigned());
#else
        return m_elements[id];
#endif
    }

    const ElementT& getById(TypeId<ContextT> id) const
    {
#ifndef SHARED_TYPE_ID
        return m_elements.at(id.toUnsigned());
#else
        static ElementT defaultReturn = {};
        auto iter = m_elements.find(id);
        if (iter == m_elements.end())
            return defaultReturn;
        else
            return iter->second;
#endif
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
#ifndef SHARED_TYPE_ID
        m_elements[id.toUnsigned()] = std::forward<ValueT>(element);
#else
        m_elements[id] = std::forward<ValueT>(element);
#endif
    }

    template <typename T, typename ValueT>
    void set(ValueT&& element)
    {
        setById(GetTypeId<ContextT, T>::value(), std::forward<ValueT>(element));
    }

private:
    TypeMapListT m_elements;
};

} // flappy
