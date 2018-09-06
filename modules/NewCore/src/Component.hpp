#pragma once

#include <Handle.hpp>

namespace flappy {

class Entity;

template <typename ... Args>
class PutAfter {
    std::vector<TypeId> previousObjects() { return std::vector { getTypeId<Args>()... }; }
};

class Component {
public:
    Component(const Handle<Entity>& entity)
        : m_entity(entity)
    {}

    Handle<Entity> entity() { return m_entity; }

private:
    Handle<Entity> m_entity;
};

} // flappy
