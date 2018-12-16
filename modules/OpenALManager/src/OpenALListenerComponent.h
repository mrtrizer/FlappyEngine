#pragma once

#include <Handle.hpp>
#include <EnableSelfHandle.hpp>

namespace flappy {
    
class OpenALManager;

class Entity;
    
    class OpenALListenerComponent : public EnableSelfHandle<OpenALListenerComponent> {
    public:
        OpenALListenerComponent(Handle<Entity> entity);
        ~OpenALListenerComponent();
        
        Handle<Entity> entity() const { return m_entity; }
        
    private:
        Handle<Entity> m_entity;
        Handle<OpenALManager> m_openALManager;
    };

} // flappy
