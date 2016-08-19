#include "ballctrl.h"

#include <core/transform.h>
#include <core/sprite.h>

namespace game {

using namespace flappy;
using namespace glm;

void BallCtrl::update(TimeDelta dt) {
    slideSpeed += dt * (slideSpeed > 0?-5:5);
    entity()->transform()->move(vec3(0, speed * dt, 0));
    entity()->transform()->move(vec3(slideSpeed * dt, 0, 0));
    if (entity()->transform()->pos().y > 40)
        EM->remove(entity());
}

} // game
