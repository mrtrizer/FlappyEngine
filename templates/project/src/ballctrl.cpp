#include "ballctrl.h"

#include <core/transform.h>

namespace game {

using namespace glm;

void BallCtrl::update(TimeDelta dt) {
    m_slideSpeed += dt * (m_slideSpeed > 0?-5:5);
    entity()->transform()->move(vec3(0, m_speed * dt, 0));
    entity()->transform()->move(vec3(m_slideSpeed * dt, 0, 0));
    if (entity()->transform()->pos().y > 40)
        EM::remove(entity());
}

} // game
