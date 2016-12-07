#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <SceneManager.h>
#include <Scene.h>

using namespace flappy;
using namespace std;
using namespace fakeit;

TEST_CASE( "SceneManager::setScene() SceneManager::scene()") {

    Mock<Scene> mock;
    Fake(Method(mock,init));

    auto managerList = std::make_shared<ManagerList>();
    auto sceneManager = managerList->create<SceneManager>();
    auto scene = std::shared_ptr<Scene>(&mock.get(), [](auto){});
    sceneManager->setScene(scene);

    Verify(Method(mock,init)).Exactly(1);

    REQUIRE(sceneManager->scene() == scene);
}
