#pragma once

#include <Handle.hpp>

namespace flappy {

class Entity;

class PutAfterMarker {
};

template <typename TypeT>
class PutAfter : public PutAfterMarker {
public:
    using PutAfterType = TypeT;
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
