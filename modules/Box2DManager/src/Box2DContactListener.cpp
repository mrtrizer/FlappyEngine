#include "Box2DContactListener.h"

#include <Tools.h>

namespace flappy {

Box2DContactListener::Box2DContactListener()
{}

void Box2DContactListener::BeginContact(b2Contact* contact) {
    LOGI("Begin contact x: %f y: %f", contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y);
    if (m_contactStartCallback != nullptr)
        m_contactStartCallback(contact);
}

void Box2DContactListener::EndContact(b2Contact* contact) {
    LOGI("End contact x: %f y: %f", contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y);
    if (m_contactEndCallback != nullptr)
        m_contactEndCallback(contact);
}

void Box2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

}

void Box2DContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {

}

} // flappy
