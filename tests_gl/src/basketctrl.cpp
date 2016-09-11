#include "basketctrl.h"

#include <core/entitymanager.h>
#include <core/sprite.h>
#include <core/transform.h>

#include "ballctrl.h"

namespace game {

using namespace flappy;

void BasketCtrl::update(TimeDelta) {
    EM->each<BallCtrl>([this](EP e) {
        float basketR = entity()->transform()->scale().x * 0.5f;
        float minDist = e->transform()->scale().x * 0.5f + basketR;
        if (distance(e->transform()->pos(), entity()->transform()->pos()) < minDist) {
            EM->remove(e);
            if (e->get<BallCtrl>()->color == color())
                entity()->transform()->stretch(-0.1f);
        }
    });
}

} // game
