#pragma once

#include <glm/vec2.hpp>

#include <Updatable.hpp>
#include <Box2DContactListener.h>
#include <EventHandle.h>
#include <Handle.hpp>
#include <EventBus.h>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DFixtureComponent;

class [[manager]] Box2DWorldManager : public Updatable<Box2DWorldManager> {
public:

    struct RayCastData {
        std::vector<Handle<Box2DFixtureComponent>> fixtures;
    };

    struct ScaleChangedEvent : public IEvent{};

    Box2DWorldManager(Handle<Hierarchy> hierarchy);
    
    void update(DeltaTime dt);

    b2World& world() { return *m_world.get(); }

    b2Joint* createJoint(std::shared_ptr<b2JointDef> jointDef);
    void destroyJoint(b2Joint*);
    
    void destroyBody(b2Body* body);

    void setGravity(glm::vec2 gravity);

    float sizeFactor() const;
    void setSizeFactor(float sizeFactor);

    int velocityIterations() const;
    void setVelocityIterations(int velocityIterations);

    int positionIterations() const;
    void setPositionIterations(int positionIterations);

    RayCastData rayCast(glm::vec2 start, glm::vec2 end);
    
    EventBus& eventBus() { return m_eventBus; }

private:
    struct ContactEventHolder {
        EventHandle eventHandle;
    };

    EventBus m_eventBus;
    Box2DContactListener m_contactListener;
    std::unique_ptr<b2World> m_world;
    int m_velocityIterations = 6;
    int m_positionIterations = 2;
    float m_sizeFactor = 1.0f;
    std::vector<b2Body*> m_bodiesToDestroy;
    
    void destroyWaitingObjects();
};

} // flappy
