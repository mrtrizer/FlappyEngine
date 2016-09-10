#pragma once

#include <string>
#include <sstream>
#include <cinttypes>
#include <iomanip>

namespace flappy {

class Color
{
public:
    /// @example ...->setColor({1.0f, 0.0f, 0.0f, 1.0f})
    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    /// @example ...->setColor(Color::fromRGBA8(255,0,0,255))
    static Color fromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    /// @example ...->setColor(Color::fromHex(0x222489FF))
    static Color fromHex(uint32_t hex) {
        return fromRGBA8((hex >> 6 * 4) & 0xff, (hex >> 4 * 4) & 0xff, (hex >> 2 * 4) & 0xff, hex & 0xff);
    }

    void setRGBA(float r, float g, float b, float a = 1.0f) {
        setR(r);
        setG(g);
        setB(b);
        setA(a);
    }

    void setR(float r) { m_r = r; }
    void setG(float g) { m_g = g; }
    void setB(float b) { m_b = b; }
    void setA(float a) { m_a = a; }
    float r() { return m_r; }
    float g() { return m_g; }
    float b() { return m_b; }
    float a() { return m_a; }

    std::string toString() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << toRGBAHex();
        return ss.str();
    }

    uint32_t toRGBAHex() {
        uint32_t hex = 0;
        hex += uint32_t(255 * m_r) << 6 * 4;
        hex += uint32_t(255 * m_g) << 4 * 4;
        hex += uint32_t(255 * m_b) << 2 * 4;
        hex += uint32_t(255 * m_a);
        return hex;
    }

private:
    float m_r = 0.0f;
    float m_g = 0.0f;
    float m_b = 0.0f;
    float m_a = 1.0f;

    char toChar(uint8_t value) {
        return value <= 9 ? '0' + value : 'a' + value;
    }

};

} // flappy
