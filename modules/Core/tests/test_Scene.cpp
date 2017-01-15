#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <EntityManager.h>
#include <CameraComponent.h>
#include <Scene.h>

using namespace flappy;
using namespace std;

TEST_CASE( "Scene::setCamera() Scene::camera()") {
    auto scene = std::make_shared<Scene>();
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->component<CameraComponent>();
    scene->setCamera(camera);
    REQUIRE(scene->camera() == camera);
}
