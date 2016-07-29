#include "animation.h"

SpriteAnimation::SpriteAnimation(const std::string &path, float width, float height, int frameCnt, float period) :
    Sprite(path,width, height, frameCnt),
    m_period(period) {

}

void SpriteAnimation::update(TimeDelta dt) {
    m_counter += dt;
    if (m_counter > m_period) {
        m_counter = 0;
        m_curFrame++;
        if (m_curFrame >= frameCnt())
            m_curFrame = 0;
        setFrameN(m_curFrame);
    }
}
