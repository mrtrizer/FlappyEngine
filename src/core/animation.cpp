#include "animation.h"

namespace flappy {

using namespace std;

SpriteAnimation::SpriteAnimation() {

}

void SpriteAnimation::setPeriod(float period) {
    m_period = period;
    updateView();
}

void SpriteAnimation::update(TimeDelta dt) {
    m_counter += dt;
    if (m_counter > m_period) {
        m_counter = 0;
        m_curFrame++;
    }
}

} // flappy
