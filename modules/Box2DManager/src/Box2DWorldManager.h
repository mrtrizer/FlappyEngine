#pragma once

#include <Manager.h>
#include <Box2DContactListener.h>
#include <EventHandle.h>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DFixtureComponent;

class Box2DWorldManager: public Manager<Box2DWorldManager> {
public:

    struct RayCastData {
        std::vector<Box2DFixtureComponent*> fixtures;
    };

    struct Box2DWorldScaleChanged : public IEvent{};

    struct ContactEvent : public IEvent {
        SafePtr<Box2DFixtureComponent> fixture;
        glm::vec2 pos;
    };

    struct ContactStartEvent : public ContactEvent {};

    struct ContactEndEvent : public ContactEvent {};

    Box2DWorldManager();

    b2World& world() { return m_world; }

    b2Joint* createJoint(std::shared_ptr<b2JointDef> jointDef);
    void destroyJoint(b2Joint*);

    void setGravity(glm::vec2 gravity);

    float sizeFactor() const;
    void setSizeFactor(float sizeFactor);

    int velocityIterations() const;
    void setVelocityIterations(int velocityIterations);

    int positionIterations() const;
    void setPositionIterations(int positionIterations);

    RayCastData rayCast(glm::vec2 start, glm::vec2 end);

private:
    struct ContactEventHolder {
        SafePtr<Entity> entity;
        EventHandle eventHandle;
    };

    template <typename ContactEventT>
    EventHandle createContactEvent(b2Contact* contact, SafePtr<Box2DFixtureComponent> otherFixture);
    template <typename ContactEventT>
    void sendContactEvent(b2Contact* contact);
    void sendContactEvents();
    void update(DeltaTime dt);

    Box2DContactListener m_contactListener;
    b2World m_world;
    int m_velocityIterations = 6;
    int m_positionIterations = 2;
    float m_sizeFactor = 1.0f;
    std::list<ContactEventHolder> m_contactEventHolders;
};

} // flappy
