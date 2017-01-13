#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <ManagerList.h>
#include <CameraComponent.h>
#include <ScreenManager.h>
#include <EntityManager.h>
#include <SceneManager.h>
#include <Scene.h>

namespace {

class TestScene: public flappy::Scene {

};

}

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "CameraComponent::rect()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->create<CameraComponent>();
    camera->setSize({0,20});
    REQUIRE(camera->rect().x1 == 0);
    REQUIRE(camera->rect().x2 == 0);
    REQUIRE(camera->rect().y1 == 0);
    REQUIRE(camera->rect().y2 == 0);
    auto screenManager = managerList->create<ScreenManager>();
    screenManager->resize(5, 10);
    REQUIRE(camera->rect().x1 == -5);
    REQUIRE(camera->rect().x2 == 5);
    REQUIRE(camera->rect().y1 == 10);
    REQUIRE(camera->rect().y2 == -10);
}

TEST_CASE( "CameraComponent::pMatrix()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->create<CameraComponent>();
    auto screenManager = managerList->create<ScreenManager>();
    screenManager->resize(5, 10);
    const float near = -1.0f;
    const float far = 99.0f;
    auto curRect = camera->rect();
    REQUIRE(camera->pMatrix() == ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far));
}

TEST_CASE( "CameraComponent::screenToScene()") {

}

TEST_CASE( "CameraComponent::init()") {
    auto managerList = std::make_shared<ManagerList>();
    auto sceneManager = managerList->create<SceneManager>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera1 = entity->create<CameraComponent>(); //Without Scene
    auto testScene = std::make_shared<TestScene>();
    sceneManager->setScene(testScene);
    sceneManager->update(1);
    auto camera2 = entity->create<CameraComponent>(); //With TestScene
    testScene->setCamera(camera2);
    REQUIRE(sceneManager->scene()->camera() == camera2);
}
