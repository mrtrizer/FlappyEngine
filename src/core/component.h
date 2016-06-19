#ifndef COMPONENT_H
#define COMPONENT_H

#include <entityx/entityx.h>

class Component
{
public:
    Component(const entityx::Entity& e):
        e(e)
    {}

protected:
    entityx::Entity e;
};

template <typename ImplBase>
class ComponentSlot: public Component {
public:
    using Component::Component;

    /// Set implementation
    inline void setImpl(const std::shared_ptr<ImplBase> & impl) { this->impl = impl; }

    inline std::shared_ptr<ImplBase> getImpl() const { return impl; }

private:
    std::shared_ptr<ImplBase> impl;
};

#endif // COMPONENT_H
