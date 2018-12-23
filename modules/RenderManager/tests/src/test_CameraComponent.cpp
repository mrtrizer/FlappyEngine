#include "catch.h"
#include "fakeit.h"

#include <memory>

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
using namespace glm;

TEST_CASE( "CameraComponent::rect()") {
    auto hierarchy = Heap::create<Hierarchy>();
    hierarchy->rootEntity()->component<MeshComponent>()->setVertices(std::vector<glm::vec3>());
}

TEST_CASE( "CameraComponent::pMatrix()") {
}

TEST_CASE( "CameraComponent::screenToScene()") {

}

TEST_CASE( "CameraComponent::init()") {

}
