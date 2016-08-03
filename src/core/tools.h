#pragma once

#include <memory>
#include <vector>

namespace flappy {

using namespace std;

#ifndef ANDROID_JNI
#include <cstdio>
template <typename ... Args>
void LOGI(Args ... args) {
    printf(args...);
}

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
    typedef vector<float> PMatrix;
    bool isPowOfTwo(int x);

    class not_implemented {};
    class unknown_intersect_obj {};
}

using TimeDelta = float;

} // flappy
