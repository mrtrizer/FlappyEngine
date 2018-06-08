#pragma once

#include <memory>
#include <vector>
#include <cstdio>
#include <iostream>
#include <regex>
#include <list>
#include <execinfo.h>

#include <glm/vec2.hpp>

#include "DebugServices.h"

namespace flappy {

template <typename Type>
std::string typeName() {
    std::regex regex("Type = ([0-9A-Za-z:<>, ]*)");
    std::smatch match;
    std::string str(__PRETTY_FUNCTION__);
    std::regex_search(str, match, regex);
    return match[1].str();
}

/// Gives some common tools
namespace Tools
{
    std::string format(const char* format, ...);

    void printStackTrace();

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

    std::string joinPath(std::vector<std::string> pathList);

    /// Check is unsigned int a power of two
    bool isPowOfTwo(unsigned x);

    /// Nearest power of two greater or equal to value
    unsigned nextHighestPowOfTwo32(unsigned value);

    template <typename T>
    T& resizeAndGet(std::vector<T>& vector, unsigned pos) {
        if (vector.size() <= pos)
            vector.resize(pos + 1);
        return vector.at(pos);
    }

    template <typename T>
    std::list<T> concat(std::list<T> first, std::list<T> second) {
        std::list<T> newList;
        newList.merge(first);
        newList.merge(second);
        return newList;
    }

    std::vector<std::string> split(const std::string &s, char delimiter);
}

using DeltaTime = float;

} // flappy
