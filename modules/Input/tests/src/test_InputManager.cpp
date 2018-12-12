#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <EventBus.h>
#include <Hierarchy.hpp>
#include <MouseInputManager.h>
#include <Heap.hpp>

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "MouseInputManager::isMouseDown()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<EventBus>();
    auto updateManager = hierarchy->initManager<UpdateManager>();
    auto inputManager = hierarchy->initManager<MouseInputManager>();
    inputManager->setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMouseDown() == false);
    updateManager->update(1.0f);
    REQUIRE(inputManager->isMouseDown() == true);
    updateManager->update(1.0f);
    REQUIRE(inputManager->isMouseDown() == false);
}

TEST_CASE( "MouseInputManager::isMouseUp()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<EventBus>();
    auto updateManager = hierarchy->initManager<UpdateManager>();
    auto inputManager = hierarchy->initManager<MouseInputManager>();
    inputManager->setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMouseUp() == false);
    updateManager->update(1.0f);
    REQUIRE(inputManager->isMouseUp() == true);
    updateManager->update(1.0f);
    REQUIRE(inputManager->isMouseUp() == false);
}

TEST_CASE( "MouseInputManager::isMousePressed()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<EventBus>();
    auto inputManager = hierarchy->initManager<MouseInputManager>();
    REQUIRE(inputManager->isMousePressed() == false);
    inputManager->setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMousePressed() == true);
    inputManager->setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager->isMousePressed() == false);
}

TEST_CASE( "MouseInputManager::getMousePos()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<EventBus>();
    hierarchy->initManager<UpdateManager>();
    auto inputManager = hierarchy->initManager<MouseInputManager>();
    REQUIRE(inputManager->mousePos() == vec2(0,0));
    inputManager->setMousePos(MouseInputManager::MouseButton::LEFT, {10, 10});
    REQUIRE(inputManager->mousePos() == vec2(10,10));
}
