#pragma once

#include <memory>

#include "tools.h"

namespace flappy {

using namespace std;

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

    shared_ptr<Entity> entity() const { return m_entity.lock(); }
private:
    weak_ptr<Entity> m_entity;
};

} // flappy
