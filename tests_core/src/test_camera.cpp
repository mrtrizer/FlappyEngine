#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <core/managerlist.h>
#include <core/camera.h>
#include <core/screenmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>

#include "testscene.h"

using namespace flappy;
using namespace std;
using namespace glm;

TEST_CASE( "Camera::rect()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->create<Camera>();
    camera->setHeight(20);
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

TEST_CASE( "Camera::pMatrix()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->create<Camera>();
    auto screenManager = managerList->create<ScreenManager>();
    screenManager->resize(5, 10);
    const float near = -1.0f;
    const float far = 99.0f;
    auto curRect = camera->rect();
    REQUIRE(camera->pMatrix() == ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far));
}

TEST_CASE( "Camera::screenToScene()") {

}

TEST_CASE( "Camera::init()") {
    auto managerList = std::make_shared<ManagerList>();
    auto sceneManager = managerList->create<SceneManager>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera1 = entity->create<Camera>(); //Without Scene
    sceneManager->setScene(std::make_shared<TestScene>());
    auto camera2 = entity->create<Camera>(); //With TestScene
    REQUIRE(sceneManager->scene()->camera() == camera2);
}
