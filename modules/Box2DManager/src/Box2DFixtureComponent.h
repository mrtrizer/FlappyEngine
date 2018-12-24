#pragma once

#include <glm/vec2.hpp>

#include <Box2D/Box2D.h>
#include <EventBus.h>
#include <Handle.hpp>
#include <Box2DContactListener.h>
#include <EnableSelfHandle.hpp>

namespace flappy {

class Box2DBodyManager;
class Entity;
    
class [[component]] Box2DFixtureComponent
    : public EnableSelfHandle<Box2DFixtureComponent>
{
public:
    struct ContactEvent : public IEvent {
        Handle<Box2DFixtureComponent> fixture;
        glm::vec2 pos;
    };
    
    struct ContactStartEvent : public ContactEvent {};
    
    struct ContactEndEvent : public ContactEvent {};
    
    void setBodyComponent(Handle<Box2DBodyManager> body);
    void resetBodyComponent(Handle<Box2DBodyManager> body);
    
    void setEntity(Handle<Entity> entity);
    ~Box2DFixtureComponent();

    float friction() const { return m_friction; }
    void setFriction(float friction);

    /// Also known as elasticity or bounciness
    float restitution() const { return m_elasticity; }
    void setRestitution(float restitution);

    float density() const { return m_density; }
    void setDensity(float density);

    bool isSensor() const { return m_isSensor; }
    void setIsSensor(bool isSensor);

    uint16_t categoryBits() const { return m_categoryBits; }
    void setCategoryBits(const uint16_t &categoryBits);

    uint16_t maskBits() const { return m_maskBits; }
    void setMaskBits(const uint16_t &maskBits);

    int16_t groupIndex() const { return m_groupIndex; }
    void setGroupIndex(const int16_t &groupIndex);
    
    void handleContact(b2Contact* contact, Box2DContactListener::ContactPhase contactPhase, Handle<Box2DFixtureComponent> other);

    EventBus& eventBus() { return m_eventBus; }
    
protected:
    void setShape(std::shared_ptr<b2Shape> shape);

private:
    void deinitFixture(Handle<Box2DBodyManager> body);
    void initFixture(Handle<Box2DBodyManager> body, std::shared_ptr<b2Shape> shape);

    Handle<Box2DBodyManager> m_box2dBodyComponent;
    std::shared_ptr<b2Shape> m_shape;

    EventBus m_eventBus;
    float m_friction = 0.2f;
    float m_elasticity = 0.0f;
    float m_density = 0.0f;
    bool m_isSensor = false;

    uint16_t m_categoryBits = 0x0001;
    uint16_t m_maskBits = 0xFFFF;
    int16_t m_groupIndex = 0;

    b2Fixture* m_fixture = nullptr;
    Handle<Box2DFixtureComponent> m_selfHandle;
};

} // flappy
