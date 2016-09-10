#pragma once

#include <memory>
#include <vector>
#include <cstdio>
#include <iostream>

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

#define LOGI(...)  std::printf(__VA_ARGS__)

template <typename ... Args>
void LOGE(Args ... args) {
    LOGI(args...);
}

#else
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

/// Gives some common tools
namespace Tools
{
    struct Rect {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    struct ColorRGBA {
        float r;
        float g;
        float b;
        float a;
    };

    using PMatrix = std::vector<float>;
    bool isPowOfTwo(int x);

    class not_implemented {};
    class unknown_intersect_obj {};
}

using TimeDelta = float;

} // flappy
