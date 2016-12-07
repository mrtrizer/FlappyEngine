#include "BasketCtrl.h"

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>

#include "BallCtrl.h"

namespace game {

using namespace flappy;

void BasketCtrl::update(TimeDelta) {
    EM->each<BallCtrl>([this](EP e) {
        float basketR = entity()->transform()->scale().x * 0.5f;
        float minDist = e->transform()->scale().x * 0.5f + basketR;
        if (distance(e->transform()->pos(), entity()->transform()->pos()) < minDist) {
            EM->remove(e);
            if (e->get<BallCtrl>()->color == color())
                entity()->transform()->stretch(-1.0f);
        }
    });
}

} // game