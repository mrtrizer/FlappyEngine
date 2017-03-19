#include "Color.h"

#include <sstream>
#include <iomanip>

namespace flappy {

Color::Color(float r, float g, float b, float a):
    m_r(r),
    m_g(g),
    m_b(b),
    m_a(a)
{
}

Color Color::fromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

Color Color::fromHex(uint32_t hex) {
    return fromRGBA8((hex >> 6 * 4) & 0xff, (hex >> 4 * 4) & 0xff, (hex >> 2 * 4) & 0xff, hex & 0xff);
}

void Color::setRGBA(float r, float g, float b, float a) {
    setR(r);
    setG(g);
    setB(b);
    setA(a);
}

std::string Color::toString() const {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << toRGBAHex();
    return ss.str();
}

uint32_t Color::toRGBAHex() const {
    uint32_t hex = 0;
    hex += uint32_t(255 * m_r) << 6 * 4;
    hex += uint32_t(255 * m_g) << 4 * 4;
    hex += uint32_t(255 * m_b) << 2 * 4;
    hex += uint32_t(255 * m_a);
    return hex;
}

} // flappy
