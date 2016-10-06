#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/entitymanager.h>
#include <core/camera.h>
#include <core/scene.h>

using namespace flappy;
using namespace std;

TEST_CASE( "Scene::setCamera() Scene::camera()") {
    auto scene = std::make_shared<Scene>();
    auto managerList = std::make_shared<ManagerList>();
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create();
    auto camera = entity->create<Camera>();
    scene->setCamera(camera);
    REQUIRE(scene->camera() == camera);
}
