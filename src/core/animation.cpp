#include "animation.h"

namespace flappy {

using namespace std;

SpriteAnimation::SpriteAnimation(const string &path, float width, float height, float period) :
    Sprite(path,width, height),
    m_period(period) {

}

void SpriteAnimation::update(TimeDelta dt) {
    m_counter += dt;
    if (m_counter > m_period) {
        m_counter = 0;
        m_curFrame++;
    }
}

} // flappy
