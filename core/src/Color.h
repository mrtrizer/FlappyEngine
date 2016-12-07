#pragma once

#include <string>
#include <cinttypes>

namespace flappy {

class Color
{
public:
    Color() = default;

    /// @example ...->setColor({1.0f, 0.0f, 0.0f, 1.0f})
    Color(float r, float g, float b, float a = 1.0f);

    /// @example ...->setColor(Color::fromRGBA8(255,0,0,255))
    static Color fromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    /// @example ...->setColor(Color::fromHex(0x222489FF))
    static Color fromHex(uint32_t hex);

    void setRGBA(float r, float g, float b, float a = 1.0f);

    void setR(float r) { m_r = r; }
    void setG(float g) { m_g = g; }
    void setB(float b) { m_b = b; }
    void setA(float a) { m_a = a; }
    float r() const { return m_r; }
    float g() const { return m_g; }
    float b() const { return m_b; }
    float a() const { return m_a; }

    std::string toString() const;
    uint32_t toRGBAHex() const;

private:
    float m_r = 1.0f;
    float m_g = 1.0f;
    float m_b = 1.0f;
    float m_a = 1.0f;
};

} // flappy
