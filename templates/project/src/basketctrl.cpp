#include "basketctrl.h"

#include <core/entitymgr.h>
#include <core/sprite.h>
#include <core/transform.h>

#include "ballctrl.h"

namespace game {

using namespace flappy;

void BasketCtrl::update(TimeDelta) {
    EM->each<BallCtrl>([this](EP e) {
        float basketR = entity()->get<Sprite>()->size().x * 0.5f * entity()->transform()->scale().x;
        float minDist = e->get<Sprite>()->size().x * 0.5f + basketR;
        if (distance(e->transform()->pos(), entity()->transform()->pos()) < minDist) {
            EM->remove(e);
            if (e->get<BallCtrl>()->color == color)
                entity()->transform()->stretch(-0.1f);
        }
    });
}

} // game
