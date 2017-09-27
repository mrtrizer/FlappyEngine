#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <Component.h>
#include <TransformComponent.h>

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
    rootEntity->component<Box2DWorldManager>()->setGravity({0.0f, -10.0f});

    // Ground
    auto groundEntity = rootEntity->createEntity();
    groundEntity->component<Box2DBodyComponent>()->body().SetType(b2_staticBody);
    groundEntity->component<TransformComponent>()->setPos({0.0f, -10.0f, 0.0f});
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundEntity->component<Box2DFixtureComponent>()->setShape(groundBox);

    // Dynamic box
    auto dynamicEntity = rootEntity->createEntity();
    dynamicEntity->component<Box2DBodyComponent>()->body().SetType(b2_dynamicBody);
    dynamicEntity->component<TransformComponent>()->setPos({0.0f, 4.0f, 0.0f});
    dynamicEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
    dynamicEntity->component<Box2DFixtureComponent>()->setDensity(1.0f);
    dynamicEntity->component<Box2DFixtureComponent>()->setFriction(0.3f);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    dynamicEntity->component<Box2DFixtureComponent>()->setShape(dynamicBox);

    float32 timeStep = 1.0f / 60.0f;

    // This is our little game loop.
    for (int32 i = 0; i < 60; ++i)
    {
        flappy::Box2DBodyComponent::UpdateEvent updateEvent(timeStep);
        rootEntity->events()->post(updateEvent);

        // Now print the position and angle of the body.
        auto position = dynamicEntity->component<TransformComponent>()->pos();
        float angle = dynamicEntity->component<TransformComponent>()->angle2DRad();

        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}
