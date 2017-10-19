#pragma once

#include <functional>

#include <Box2D.h>

#include <EventHandle.h>

namespace flappy {

class Box2DContactListener: public b2ContactListener {
public:
    Box2DContactListener();

    void BeginContact(b2Contact* contact) final;
    void EndContact(b2Contact* contact) final;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) final;
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) final;

    void setContactStartCallback(std::function<void(b2Contact*)> callback) {
        m_contactStartCallback = callback;
    }
    void setContactEndCallback(std::function<void(b2Contact*)> callback) {
        m_contactEndCallback = callback;
    }

private:
    std::function<void(b2Contact*)> m_contactStartCallback;
    std::function<void(b2Contact*)> m_contactEndCallback;
};

} // flappy
