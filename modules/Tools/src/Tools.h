#pragma once

#include <memory>
#include <vector>
#include <cstdio>
#include <iostream>
#include <regex>
#include <list>
#include <execinfo.h>

#include <glm/vec2.hpp>

namespace flappy {

inline void LOGI_default(const char* s) {
    while (*s) {
        if (*s == '%' && *++s != '%')
            return;
        std::cout << *s++;
    }
}

template<typename T, typename... Args>
void LOGI_default(const char* s, const T& value, const Args&... args) {
    while (*s) {
        if (*s == '%' && *++s != '%') {
            std::cout << value;
            return LOGI_default(++s, args...);
        }
        std::cout << *s++;
    }
}

#ifndef ANDROID_JNI

#define LOGI(...) std::printf("\x1b[32m [INFO] \x1b[0m"), std::printf(__VA_ARGS__), std::printf("\n")
#define LOGW(...) std::printf("\x1b[33m [WARNING] \x1b[0m"), std::printf(__VA_ARGS__), std::printf("\n")
#define LOGE(...) std::printf("\x1b[31m [ERROR] \x1b[0m"), std::printf(__VA_ARGS__), std::printf("\n")

#else
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARNING,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#define VOID_VALUE void()
#define ERROR_MSG(value, ...) LOGE(__VA_ARGS__), LOGE("%s:%d\n", __FILE__, __LINE__), value;

template <typename Type>
std::string typeName() {
    std::regex regex("Type = (.*)]");
    std::smatch match;
    std::string str(__PRETTY_FUNCTION__);
    std::regex_search(str, match, regex);
    return match[1].str();
}

/// Gives some common tools
namespace Tools
{
    void printStackTrace();

    struct Rect {
        Rect()
        {}
        Rect(float x1, float y1, float x2, float y2):
            x1(x1), y1(y1), x2(x2), y2(y2)
        {}
        bool operator==(const Rect& rect);
        glm::vec2 size() {return {x2 - x1, y2 - y1};}
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
}

using DeltaTime = float;

} // flappy
