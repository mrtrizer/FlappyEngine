#include "Box2DContactListener.h"

namespace flappy {

void Box2DContactListener::BeginContact(b2Contact* contact) {
    if (m_contactCallback != nullptr)
        m_contactCallback(contact, ContactPhase::BEGIN);
}

void Box2DContactListener::EndContact(b2Contact* contact) {
    if (m_contactCallback != nullptr)
        m_contactCallback(contact, ContactPhase::END);
}

void Box2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

}

void Box2DContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {

}

} // flappy
