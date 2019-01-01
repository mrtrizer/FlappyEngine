#pragma once

#include <functional>

#include <Updatable.hpp>
#include <EnableSelfHandle.hpp>

namespace flappy {
    
    class Scheduler : public Updatable<Scheduler>, public EnableSelfHandle<Scheduler> {
    public:
        using Updatable<Scheduler>::Updatable;
        
        ~Scheduler() {
        
        }
    
        void setEntity(Handle<Entity> entity) {
            m_entity = entity;
        }
    
        void schedule(float delaySec, const std::function<void()>& callback) {
            m_timeLeftSec = delaySec;
            m_callback = callback;
        }
        
        void update(DeltaTime dt) {
            m_timeLeftSec -= dt;
            if (m_timeLeftSec <= 0.0f) {
                m_entity->removeComponent(selfHandle());
                m_callback();
            }
        }
        
    private:
        float m_timeLeftSec = 0.0f;
        std::function<void()> m_callback;
        Handle<Entity> m_entity;
    };
    
} // flappy
