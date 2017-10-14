#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <TransformComponent.h>
#include <UIManager.h>
#include <TouchComponent.h>
#include <Box2DBodyComponent.h>
#include <Box2DBoxComponent.h>
#include <Box2DWorldManager.h>
#include <MouseInputManager.h>
#include <TouchInputManager.h>
#include <MouseToTouchConvertComponent.h>

#include "ButtonListenerComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;
using namespace glm;

TEST_CASE("UIManager") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<UIManager>();
    auto touchInputManager = rootEntity->createComponent<TouchInputManager>();
    rootEntity->createComponent<Box2DWorldManager>();

    auto buttonEntity = rootEntity->createEntity();
    buttonEntity->createComponent<TransformComponent>();
    buttonEntity->createComponent<Box2DBodyComponent>();
    buttonEntity->component<Box2DBoxComponent>()->setSize({100, 50});
    auto touchComponent = buttonEntity->createComponent<TouchComponent>();

    Mock<ButtonListenerComponent::IMock> mock;
    Fake(Method(mock,onTouchDown));
    Fake(Method(mock,onTouchUp));
    Fake(Method(mock,onTouchMove));

    auto buttonListenerComponent = buttonEntity->createComponent<ButtonListenerComponent>(&mock.get());

    // button hit

    touchInputManager->setTouchDown({0, 0}, 0);

    Verify(Method(mock,onTouchDown)).Exactly(1);

    touchInputManager->setTouchPos({1, 0}, 0);

    Verify(Method(mock,onTouchMove)).Exactly(1);

    touchInputManager->setTouchUp({0, 0}, 0);

    Verify(Method(mock,onTouchUp)).Exactly(1);

    // button miss

    touchInputManager->setTouchDown({51, 0}, 0);

    Verify(Method(mock,onTouchDown)).Exactly(1);

    touchInputManager->setTouchPos({-51, 0}, 0);

    Verify(Method(mock,onTouchMove)).Exactly(1);

    touchInputManager->setTouchUp({0, 26}, 0);

    Verify(Method(mock,onTouchUp)).Exactly(1);
}

