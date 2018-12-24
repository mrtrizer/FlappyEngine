#pragma once

#include <vector>
#include <cmath>

#include <glm/vec2.hpp>

namespace flappy {

/// Gives some common tools
namespace MathUtils
{
    struct Rect {
        Rect() = default;
        Rect(float x1, float y1, float x2, float y2)
            : x1(x1)
            , y1(y1)
            , x2(x2)
            , y2(y2)
        {}
        glm::vec2 size() const {return {std::fabs(x2 - x1), std::fabs(y2 - y1)};}
        float x1 = 0.0f;
        float y1 = 0.0f;
        float x2 = 0.0f;
        float y2 = 0.0f;
    };
    
    /// Check is unsigned int a power of two
    bool isPowOfTwo(unsigned x);

    /// Nearest power of two greater or equal to value
    uint32_t nextHighestPowOfTwo32(uint32_t value);
}

} // flappy
