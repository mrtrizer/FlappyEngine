#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class Entity;

using TimeDelta = float;

class Component {
    friend class Entity;
public:
    virtual ~Component() {}

    virtual void update(TimeDelta) {}
    virtual void init() {}

    std::shared_ptr<Entity> entity() const { return m_entity.lock(); }
private:
    std::weak_ptr<Entity> m_entity;
};

#endif // COMPONENT_H
