#include <cmath>
#include <functional>

#include "Tools.h"

namespace flappy {

using namespace std;

/// Common tools for model
namespace Tools {

void printStackTrace() {
    void* callstack[128];
    int frames = backtrace(callstack, sizeof(callstack) / sizeof(void*));
    char** strs = backtrace_symbols(callstack, frames);
    for (int i = 0; i < (frames - 1); ++i) {
        printf("%s\n", strs[i]);
    }
    free(strs);
}

bool Rect::operator==(const Rect& rect)
{
    return x1 == rect.x1
            && y1 == rect.y1
            && x2 == rect.x2
            && y2 == rect.y2;
}

/// http://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
bool isPowOfTwo(int x) {
    return !(x == 0) && !(x & (x - 1));
}

} // Tools

} // flappy
