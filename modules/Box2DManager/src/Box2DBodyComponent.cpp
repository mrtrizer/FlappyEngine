#include "Box2DBodyComponent.h"

#include <TransformComponent.h>
#include <Entity.h>

#include "Box2DWorldManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DBodyComponent::Box2DBodyComponent() {
    addDependency(Box2DWorldManager::id());

    events()->subscribe([this](InitEvent) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.0f, 0.0f);

        m_lastTransformPos = entity()->component<TransformComponent>()->pos();
        m_lastTransformAngle = entity()->component<TransformComponent>()->angle2DRad();

        m_body = manager<Box2DWorldManager>()->world().CreateBody(&bodyDef);
    });

    events()->subscribe([this](DeinitEvent) {
        manager<Box2DWorldManager>()->world().DestroyBody(m_body);
    });

    events()->subscribe([this](UpdateEvent e) {
        update(e.dt);
    });
}

b2Body& Box2DBodyComponent::body() {
    if (m_body == nullptr)
        throw std::runtime_error("Box2DWorldManager is not found in hierarchy.");
    return *m_body;
}

bool Box2DBodyComponent::testPoint(glm::vec2 point) {
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
    b2Vec2 b2Point(point.x * sizeFactor, point.y * sizeFactor);
    b2Fixture* fixture = body().GetFixtureList();
    while (fixture) {
        if (fixture->TestPoint(b2Point)) {
            return true;
        }
        fixture = fixture->GetNext();
    }
    return false;
}

void Box2DBodyComponent::update(DeltaTime dt) {
    glm::vec3 newTransformPos = entity()->component<TransformComponent>()->pos();
    float newTransformAngle = entity()->component<TransformComponent>()->angle2DRad();

    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();

    if ((m_lastTransformAngle == newTransformAngle) && (m_lastTransformPos == newTransformPos)) {
        auto box2DPosition = body().GetPosition();
        entity()->component<TransformComponent>()->setPos(glm::vec3(box2DPosition.x, box2DPosition.y, m_lastTransformPos.z) / sizeFactor);
        entity()->component<TransformComponent>()->setAngle2DRad(body().GetAngle());
    } else {
        body().SetTransform(b2Vec2(newTransformPos.x * sizeFactor, newTransformPos.y * sizeFactor), newTransformAngle);
    }

    m_lastTransformPos = entity()->component<TransformComponent>()->pos();
    m_lastTransformAngle = entity()->component<TransformComponent>()->angle2DRad();
}

} // flappy
