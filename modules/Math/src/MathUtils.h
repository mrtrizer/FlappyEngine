#pragma once

#include <vector>

#include <glm/vec2.hpp>

namespace flappy {

/// Gives some common tools
namespace MathUtils
{
    struct Rect {
        Rect()
        {}
        Rect(float x1, float y1, float x2, float y2):
        x1(x1), y1(y1), x2(x2), y2(y2)
        {}
        bool operator==(const Rect& rect) const;
        glm::vec2 size() const {return {x2 - x1, y2 - y1};}
        float x1 = 0.0f;
        float y1 = 0.0f;
        float x2 = 0.0f;
        float y2 = 0.0f;
    };
    
    using PMatrix = std::vector<float>;
    
    
    /// Check is unsigned int a power of two
    bool isPowOfTwo(unsigned x);

    /// Nearest power of two greater or equal to value
    unsigned nextHighestPowOfTwo32(unsigned value);
}

} // flappy
