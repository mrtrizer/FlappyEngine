#pragma once

#include <functional>

#include <Box2D.h>

#include <EventHandle.h>

namespace flappy {

class Box2DContactListener: public b2ContactListener {
public:
    enum class ContactPhase {
        BEGIN = 1,
        END = 2
    };
    
    void BeginContact(b2Contact* contact) final;
    void EndContact(b2Contact* contact) final;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) final;
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) final;

    using ContactCallback = std::function<void(b2Contact*, ContactPhase)>;
    
    void setContactCallback(const ContactCallback& callback) { m_contactCallback = callback; }

private:
    ContactCallback m_contactCallback;
};

} // flappy
