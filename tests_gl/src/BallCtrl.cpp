#include "BallCtrl.h"

#include <TransformComponent.h>
#include <SpriteComponent.h>
#include <PresenterComponent.h>

namespace game {

using namespace flappy;
using namespace glm;

void BallCtrl::update(TimeDelta dt) {
    slideSpeed += dt * (slideSpeed > 0?-5:5);
    component<TransformComponent>()->move({0.0f, speed * dt});
    component<TransformComponent>()->move({slideSpeed * dt, 0.0f});
    component<TransformComponent>()->rotate(dt);
    if (auto shape = component<CircleShape>())
        shape->setColor({shape->color().r() + 0.1f * dt, 0, 0, shape->color().a()});
    auto scale = component<TransformComponent>()->scale();
    component<TransformComponent>()->setScale({scale.x, scale.y + dt});
    if (component<TransformComponent>()->pos().y > 320)
        manager<EntityManager>()->remove(entity().lock());
}

} // game
