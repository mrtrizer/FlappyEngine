#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <TransformComponent.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "TransformComponent angle 2D radians" ) {
    auto transformComponent = std::make_shared<TransformComponent>();
    transformComponent->setAngle2DRad(2.0f);
    REQUIRE(transformComponent->angle2DRad() == 2.0f);
    transformComponent->rotate2DRad(1.0f);
    REQUIRE(transformComponent->angle2DRad() == 3.0f);
}

TEST_CASE( "TransformComponent position" ) {
    auto transformComponent = std::make_shared<TransformComponent>();
    const auto pos = glm::vec3{10,-10,1};
    transformComponent->setPos(pos);
    REQUIRE(transformComponent->pos() == pos);
    transformComponent->move(pos);
    REQUIRE(transformComponent->pos() == pos + pos);
    const auto vec2Pos = glm::vec2{1,1};
    transformComponent->move(vec2Pos);
    REQUIRE(transformComponent->pos() == pos + pos + glm::vec3(vec2Pos, 0));
}
