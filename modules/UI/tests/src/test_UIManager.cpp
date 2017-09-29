#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <TransformComponent.h>
#include <UIManager.h>
#include <TouchComponent.h>
#include <Box2DBodyComponent.h>
#include <Box2DFixtureComponent.h>

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE("UIManager") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<UIManager>();
    auto buttonEntity = rootEntity->createEntity();
    buttonEntity->createComponent<TransformComponent>();
    buttonEntity->createComponent<Box2DBodyComponent>();
    auto shape = std::make_shared<b2PolygonShape>();
    shape->SetAsBox(100, 50);
    buttonEntity->component<Box2DFixtureComponent>()->setShape(shape);
    buttonEntity->createComponent<TouchComponent>();
}

