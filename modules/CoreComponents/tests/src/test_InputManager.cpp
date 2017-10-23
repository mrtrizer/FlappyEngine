#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <MouseInputManager.h>

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "MouseInputManager::isMouseDown()") {
    auto entity = std::make_shared<Entity>();
    auto inputManager = entity->createComponent<MouseInputManager>();
    inputManager->setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMouseDown() == false);
    inputManager->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(inputManager->isMouseDown() == true);
    inputManager->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(inputManager->isMouseDown() == false);
}

TEST_CASE( "MouseInputManager::isMouseUp()") {
    auto entity = std::make_shared<Entity>();
    auto inputManager = entity->createComponent<MouseInputManager>();
    inputManager->setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMouseUp() == false);
    inputManager->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(inputManager->isMouseUp() == true);
    inputManager->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(inputManager->isMouseUp() == false);
}

TEST_CASE( "MouseInputManager::isMousePressed()") {
    auto entity = std::make_shared<Entity>();
    auto inputManager = entity->createComponent<MouseInputManager>();
    REQUIRE(inputManager->isMousePressed() == false);
    inputManager->setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMousePressed() == true);
    inputManager->setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMousePressed() == false);
}

TEST_CASE( "MouseInputManager::getMousePos()") {
    auto entity = std::make_shared<Entity>();
    auto inputManager = entity->createComponent<MouseInputManager>();
    REQUIRE(inputManager->mousePos() == vec2(0,0));
    inputManager->setMousePos(MouseInputManager::MouseButton::LEFT, {10, 10});
    REQUIRE(inputManager->mousePos() == vec2(10,10));
}
