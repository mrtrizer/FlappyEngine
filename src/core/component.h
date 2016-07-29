#pragma once

#include <memory>

#include "tools.h"

class Entity;

class Component {
    friend class Entity;
public:
    Component(){}
    Component(const Component&) = delete;
    void operator=(const Component&) = delete;
    virtual ~Component() {}
    virtual void update(TimeDelta) {}
    virtual void init() {}

    std::shared_ptr<Entity> entity() const { return m_entity.lock(); }
private:
    std::weak_ptr<Entity> m_entity;
};
