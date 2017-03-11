#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <vector>
#include <stdexcept>

#include "ClassId.h"

namespace  flappy {

template <typename ContextT, typename ElementT = void>
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

    template <typename T>
    void set(T&& element)
    {
        unsigned id = ClassId<ContextT, T>::id();
        elements.emplace(id, std::forward<T>(element));
    }

private:
    std::vector<ElementT> elements;
};

} // flappy

#endif // TYPEMAP_H
