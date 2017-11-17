#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <vector>
#include <stdexcept>

#include "ClassId.h"

namespace  flappy
{

template <typename ContextT, typename ElementT>
class TypeMap
{
public:
    TypeMap() :
        m_elements(TypeCounter<ContextT>::count())
    {}

    typename std::vector<ElementT>::iterator begin()
    {
        return m_elements.begin();
    }

    typename std::vector<ElementT>::iterator end()
    {
        return m_elements.end();
    }

    typename std::vector<ElementT>::const_iterator begin() const
    {
        return m_elements.begin();
    }

    typename std::vector<ElementT>::const_iterator end() const
    {
        return m_elements.end();
    }

    ElementT& getById(TypeId<ContextT> id)
    {
        return m_elements.at(id.toUnsigned());
    }

    // TODO: Add test
    int size() {
        return int(m_elements.size());
    }

    ElementT getByName(std::string name) const
    {
        unsigned index = TypeNames<ContextT>::instance().getIndexByName(name);
        return m_elements.at(index);
    }

    ElementT getById(TypeId<ContextT> id) const
    {
        return m_elements.at(id.toUnsigned());
    }

    // TODO: Add test
    template <typename ValueT>
    void setById(TypeId<ContextT> id, ValueT&& element)
    {
        m_elements[id.toUnsigned()] = std::forward<ValueT>(element);
    }

    template <typename T>
    ElementT& get()
    {
        return getById(GetTypeId<ContextT, T>::value());
    }

    template <typename T>
    ElementT get() const
    {
        return getById(GetTypeId<ContextT, T>::value());
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

#endif // TYPEMAP_H
