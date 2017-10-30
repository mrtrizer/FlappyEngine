#include <cmath>
#include <functional>
#include <cstdio>
#include <cstdarg>
#include <sstream>

#include "Tools.h"

namespace flappy {

using namespace std;

/// Common tools for model
namespace Tools {

std::string format(const char* format, ...) {
    std::string outputBuff(5000, ' ');
    va_list arglist;
    va_start(arglist, format);
    int length = std::vsnprintf(&outputBuff.front(), outputBuff.size(), format, arglist );
    outputBuff.resize(length);
    va_end(arglist);
    return outputBuff;
}

void printStackTrace() {
    void* callstack[128];
    int frames = backtrace(callstack, sizeof(callstack) / sizeof(void*));
    char** strs = backtrace_symbols(callstack, frames);
    for (int i = 0; i < (frames - 1); ++i) {
        printf("%s\n", strs[i]);
    }
    free(strs);
}

std::string joinPath(std::vector<std::string> pathList) {
    if (pathList.size() < 1)
        return "";
    std::string outPath = pathList[0];
    for (int i = 1; i < pathList.size(); i++) {
        outPath += "/" + pathList[i];
    }
    return outPath;
}

bool Rect::operator==(const Rect& rect)
{
    return x1 == rect.x1
            && y1 == rect.y1
            && x2 == rect.x2
            && y2 == rect.y2;
}

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

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delimiter)) {
        elems.push_back(std::move(item));
    }
    return elems;
}

} // Tools

} // flappy
