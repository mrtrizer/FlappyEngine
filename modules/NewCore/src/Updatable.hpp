#pragma once

#include <Handle.hpp>

#include "Hierarchy.hpp"
#include "Entity.hpp"
#include "UpdataManager.hpp"

namespace flappy {

template <typename DerivedT>
class Updatable {
public:
    Updatable(const Handle<Hierarchy>& hierarchy)
        : m_updateManager(hierarchy->manager<UpdateManager>())
        , m_functionId(m_updateManager->registerUpdateFunction(0, [this](float dt){
                static_cast<DerivedT*>(this)->update(dt);
            }))
    {}
    Updatable(const Handle<Entity>& entity)
        : m_updateManager(entity->hierarchy()->manager<UpdateManager>())
        , m_functionId(m_updateManager->registerUpdateFunction(entity->deepness(), [this](float dt){
                static_cast<DerivedT*>(this)->update(dt);
            }))
    {}
    ~Updatable() {
        if (m_updateManager.isValid())
            m_updateManager->unregisterUpdateFunction(m_functionId);
    }
private:
    Handle<UpdateManager> m_updateManager;
    int m_functionId = 0;
};

} // flappy
