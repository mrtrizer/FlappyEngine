#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/inputmanager.h>

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "InputManager::isMouseDown()") {
    InputManager inputManager;
    inputManager.setMouseDown();
    REQUIRE(inputManager.mouseDown() == false);
    inputManager.update(1);
    REQUIRE(inputManager.mouseDown() == true);
    inputManager.update(1);
    REQUIRE(inputManager.mouseDown() == false);
}

TEST_CASE( "InputManager::isMouseUp()") {
    InputManager inputManager;
    inputManager.setMouseUp();
    REQUIRE(inputManager.mouseUp() == false);
    inputManager.update(1);
    REQUIRE(inputManager.mouseUp() == true);
    inputManager.update(1);
    REQUIRE(inputManager.mouseUp() == false);
}

TEST_CASE( "InputManager::isMousePressed()") {
    InputManager inputManager;
    REQUIRE(inputManager.mousePressed() == false);
    inputManager.setMouseDown();
    REQUIRE(inputManager.mousePressed() == true);
    inputManager.setMouseUp();
    REQUIRE(inputManager.mousePressed() == false);
}

TEST_CASE( "InputManager::getMousePos()") {
    InputManager inputManager;
    REQUIRE(inputManager.mousePos() == vec2(0,0));
    inputManager.setMousePos({10, 10});
    REQUIRE(inputManager.mousePos() == vec2(10,10));
}
