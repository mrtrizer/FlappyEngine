#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <SceneManager.h>
#include <Scene.h>

#include "TestScene.h"

using namespace flappy;
using namespace std;
using namespace fakeit;

TEST_CASE( "SceneManager::setScene() SceneManager::scene()") {

    Mock<TestScene::IMock> mock;
    Fake(Method(mock,init));

    auto managerList = std::make_shared<ManagerList>();
    auto sceneManager = managerList->create<SceneManager>();
    auto scene = std::make_shared<TestScene>(&mock.get());
    sceneManager->setScene(scene);

    Verify(Method(mock,init)).Exactly(1);

    REQUIRE(sceneManager->scene() == scene);
}
