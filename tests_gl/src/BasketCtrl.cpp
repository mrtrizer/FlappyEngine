#include "BasketCtrl.h"

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>

#include "BallCtrl.h"

namespace game {

using namespace flappy;

void BasketCtrl::update(TimeDelta) {
    manager<EntityManager>()->each<BallCtrl>([this](EP e) {
        float basketR = component<TransformComponent>()->scale().x * 0.5f;
        float minDist = e->component<TransformComponent>()->scale().x * 0.5f + basketR;
        if (distance(e->component<TransformComponent>()->pos(), component<TransformComponent>()->pos()) < minDist) {
            manager<EntityManager>()->remove(e);
            if (e->component<BallCtrl>()->color == color())
                component<TransformComponent>()->stretch(-1.0f);
        }
    });
}

} // game
