#include "catch.h"
#include "fakeit.h"

#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <CameraComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>

using namespace flappy;
using namespace std;
using namespace fakeit;

TEST_CASE( "SceneManager") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<ScreenManager>();
    auto sceneManager = hierarchy->initManager<SceneManager>();
    auto camera = hierarchy->rootEntity()->createComponent<CameraComponent>();
    REQUIRE(sceneManager->mainCamera() == camera);
}
