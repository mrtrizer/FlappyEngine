#ifndef GTOOLS_H
#define GTOOLS_H

#include <memory>

#ifdef QT_CORE_LIB
#include <QDebug>
#define  LOGI(...) qDebug(__VA_ARGS__)
#define  LOGE(...) qDebug(__VA_ARGS__)
#else
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

class GObj;
class GColliderCircle;
class GColliderRect;

/// Gives some common tools
namespace GTools
{
    typedef std::vector<float> PMatrix;

    bool isIntersect(const GObj &, const GObj &);
    bool isPowOfTwo(int x);

    class not_implemented {};
    class unknown_intersect_obj {};
}

#endif // GTOOLS_H
