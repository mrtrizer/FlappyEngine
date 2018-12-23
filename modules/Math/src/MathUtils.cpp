#include "MathUtils.h"

namespace flappy {

/// Common tools for model
namespace MathUtils {
    

/// http://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
bool isPowOfTwo(unsigned x) {
    return !(x == 0) && !(x & (x - 1));
}

/// http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2Float
unsigned nextHighestPowOfTwo32(unsigned v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    v += (v == 0);
    return v;
}

} // Tools

} // flappy
