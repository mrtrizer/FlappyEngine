#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <vector>
#include <stdexcept>

#include "ClassId.h"

namespace  flappy {

template <typename ContextT, typename ElementT>
class TypeMap
{
public:
    TypeMap() :
        elements(ClassCounter<ContextT>::count())
    {}

    ElementT& getById(unsigned id)
    {
        return elements[id];
    }

    template <typename T>
    ElementT& get()
    {
        return getById(ClassId<ContextT, T>::id());
    }

    template <typename T, typename ValueT>
    void set(ValueT&& element)
    {
        unsigned id = ClassId<ContextT, T>::id();
        elements[id] = std::forward<ValueT>(element);
    }

private:
    std::vector<ElementT> elements;
};

} // flappy

#endif // TYPEMAP_H
