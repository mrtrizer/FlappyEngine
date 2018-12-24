#include "catch.h"

#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <CameraComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>

using namespace flappy;

TEST_CASE( "SceneManager") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<ScreenManager>();
    auto sceneManager = hierarchy->initManager<SceneManager>();
    bool changed = false;
    auto subscription = sceneManager->eventBus().subscribe([&changed](SceneManager::CameraChangedEvent) {
        changed = true;
    });
    auto camera = hierarchy->rootEntity()->createComponent<CameraComponent>();
    REQUIRE(sceneManager->mainCamera() == camera);
    REQUIRE(changed == true);
}
