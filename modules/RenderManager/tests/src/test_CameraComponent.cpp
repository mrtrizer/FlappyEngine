#include "catch.h"
#include "fakeit.h"

#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#include <CameraComponent.h>
#include <MeshComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>
#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <Entity.hpp>

using namespace flappy;
using namespace std;

TEST_CASE( "CameraComponent::rect()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->initManager<SceneManager>();
    auto screenManager = hierarchy->initManager<ScreenManager>();
    screenManager->resize({120, 200});
    glm::vec2 cameraSize{100, 200};
    hierarchy->rootEntity()->component<CameraComponent>()->setSize(cameraSize);
    auto cameraRect = hierarchy->rootEntity()->component<CameraComponent>()->rect();
    std::numeric_limits<glm::vec2::type>::epsilon();
    REQUIRE(cameraRect.size().x == screenManager->screenSize().x);
    REQUIRE(cameraRect.size().y == cameraSize.y);
}

TEST_CASE( "CameraComponent::pMatrix()") {
}

TEST_CASE( "CameraComponent::screenToScene()") {

}

TEST_CASE( "CameraComponent::init()") {

}
