#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <Component.h>

#include <Box2D.h>
#include <Box2DWorldManager.h>
#include <Box2DBodyComponent.h>
#include <Box2DFixtureComponent.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE("Box2D") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->component<Box2DWorldManager>()->setVelocityIterations(6);
    rootEntity->component<Box2DWorldManager>()->setPositionIterations(2);

    // Ground
    auto groundEntity = rootEntity->createEntity();
    groundEntity->component<Box2DBodyComponent>()->body().SetType(b2_staticBody);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundEntity->component<Box2DFixtureComponent>()->setShape(groundBox);

    // Dynamic box

    auto dynamicEntity = rootEntity->createEntity();
    dynamicEntity->component<Box2DBodyComponent>()->body().SetType(b2_dynamicBody);
    dynamicEntity->component<Box2DBodyComponent>()->body().SetTransform(b2Vec2(0.0f, 4.0f), 0.0f);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    dynamicEntity->component<Box2DFixtureComponent>()->setDensity(1.0f);
    dynamicEntity->component<Box2DFixtureComponent>()->setFriction(0.3f);
    dynamicEntity->component<Box2DFixtureComponent>()->setShape(dynamicBox);

    float32 timeStep = 1.0f / 60.0f;

    // This is our little game loop.
    for (int32 i = 0; i < 60; ++i)
    {
        flappy::Box2DBodyComponent::OnUpdate updateEvent;
        updateEvent.dt = 1.0f;
        rootEntity->events()->post(updateEvent);

        // Now print the position and angle of the body.
        b2Vec2 position = dynamicEntity->component<Box2DBodyComponent>()->body().GetPosition();
        float32 angle = dynamicEntity->component<Box2DBodyComponent>()->body().GetAngle();

        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}
