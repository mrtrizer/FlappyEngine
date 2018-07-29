#pragma once

#include <Handle.hpp>

class Entity;

template <typename ... Args>
class PutAfter {
    std::vector<TypeId> previousObjects() { return std::vector { getTypeId<Args>()... }; }
};

class Component {
public:
    Component()
        : m_entity(lastEntity())
    {}

    Handle<Entity> entity() { return m_entity; }

private:
    Handle<Entity> m_entity;
};
