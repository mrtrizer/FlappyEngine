#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <MouseInputManager.h>

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "MouseInputManager::isMouseDown()") {
    MouseInputManager inputManager;
    inputManager.setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager.isMouseDown() == false);
    inputManager.update(1);
    REQUIRE(inputManager.isMouseDown() == true);
    inputManager.update(1);
    REQUIRE(inputManager.isMouseDown() == false);
}

TEST_CASE( "MouseInputManager::isMouseUp()") {
    MouseInputManager inputManager;
    inputManager.setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager.isMouseUp() == false);
    inputManager.update(1);
    REQUIRE(inputManager.isMouseUp() == true);
    inputManager.update(1);
    REQUIRE(inputManager.isMouseUp() == false);
}

TEST_CASE( "MouseInputManager::isMousePressed()") {
    MouseInputManager inputManager;
    REQUIRE(inputManager.isMousePressed() == false);
    inputManager.setMouseDown(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager.isMousePressed() == true);
    inputManager.setMouseUp(MouseInputManager::MouseButton::LEFT, {0.0f, 0.0f});
    REQUIRE(inputManager.isMousePressed() == false);
}

TEST_CASE( "MouseInputManager::getMousePos()") {
    MouseInputManager inputManager;
    REQUIRE(inputManager.mousePos() == vec2(0,0));
    inputManager.setMousePos(MouseInputManager::MouseButton::LEFT, {10, 10});
    REQUIRE(inputManager.mousePos() == vec2(10,10));
}
