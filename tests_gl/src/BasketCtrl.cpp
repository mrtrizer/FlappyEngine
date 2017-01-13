#include "BasketCtrl.h"

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>

#include "BallCtrl.h"

namespace game {

using namespace flappy;

void BasketCtrl::update(TimeDelta) {
    manager<EntityManager>()->each<BallCtrl>([this](EP e) {
        float basketR = entity()->transform()->scale().x * 0.5f;
        float minDist = e->transform()->scale().x * 0.5f + basketR;
        if (distance(e->transform()->pos(), entity()->transform()->pos()) < minDist) {
            manager<EntityManager>()->remove(e);
            if (e->component<BallCtrl>()->color == color())
                entity()->transform()->stretch(-1.0f);
        }
    });
}

} // game
