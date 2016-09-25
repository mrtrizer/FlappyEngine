#pragma once

#include <memory>
#include <vector>
#include <cstdio>
#include <iostream>

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

#define VOID_VALUE ([](){})()
#define ERROR_MSG(value, ...) LOGE(__VA_ARGS__), value;

/// Gives some common tools
namespace Tools
{
    struct Rect {
        glm::vec2 size() {return {x2 - x1, y2 - y1};}
        float x1;
        float y1;
        float x2;
        float y2;
    };

    using PMatrix = std::vector<float>;
    bool isPowOfTwo(int x);

    template <typename T>
    T& resizeAndGet(std::vector<T>& vector, unsigned pos) {
        if (vector.size() <= pos)
            vector.resize(pos + 1);
        return vector[pos];
    }

    class not_implemented {};
    class unknown_intersect_obj {};
}

using TimeDelta = float;

} // flappy
