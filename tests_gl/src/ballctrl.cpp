#include "ballctrl.h"

#include <core/transform.h>
#include <core/sprite.h>
#include <core/rectshape.h>

namespace game {

using namespace flappy;
using namespace glm;

void BallCtrl::update(TimeDelta dt) {
    slideSpeed += dt * (slideSpeed > 0?-5:5);
    entity()->transform()->move({0.0f, speed * dt});
    entity()->transform()->move({slideSpeed * dt, 0.0f});
    entity()->transform()->rotate(dt);
    if (auto shape = entity()->get<RectShape>())
        shape->setColor({shape->color().r() + 0.1f * dt, 0, 0, shape->color().a()});
    auto scale = entity()->transform()->scale();
    entity()->transform()->setScale({scale.x, scale.y + dt});
    if (entity()->transform()->pos().y > 40)
        EM->remove(entity());
}

} // game
