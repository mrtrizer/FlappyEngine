#pragma once

namespace flappy {

/// Gives some common tools
namespace Math
{
    /// Check is unsigned int a power of two
    bool isPowOfTwo(unsigned x);

    /// Nearest power of two greater or equal to value
    unsigned nextHighestPowOfTwo32(unsigned value);
}

} // flappy
