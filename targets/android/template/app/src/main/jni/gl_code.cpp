/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <gl/gltools.h>
#include <gl/gltexture.h>
#include <gl/glviewfactory.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <ctrl.h>
#include <gl/glworldview.h>

JNIEnv * gEnv = 0;
jclass gClass;
pthread_mutex_t lock;

class GLViewFactoryAndroid : public GLViewFactory {
public:
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string name) const override {
        char const *path = name.data();
        jmethodID mid;
        mid = gEnv->GetStaticMethodID(gClass, "loadBitmap",
                                 "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
        jstring pathUTF = gEnv->NewStringUTF(path);
        jobject bitmap = gEnv->CallStaticObjectMethod(gClass, mid, pathUTF);
        gEnv->DeleteLocalRef(pathUTF);
        gEnv->NewGlobalRef(bitmap);

        LOGI("Name: %s",path);
        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getWidth","()I");
        int width = gEnv->CallIntMethod(bitmap,mid);
        LOGI("Width: %d",width);
        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getHeight","()I");
        int height = gEnv->CallIntMethod(bitmap,mid);
        LOGI("Height: %d",width);
        jintArray array = gEnv->NewIntArray(width * height);
        gEnv->NewGlobalRef(array);
        //getPixels(int[] pixels, int offset, int stride, int x, int y, int width, int height)
        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getPixels","([IIIIIII)V");
        gEnv->CallVoidMethod(bitmap,mid,array,0,width,0,0,width,height);
        jint *pixels = gEnv->GetIntArrayElements(array, 0);
        gEnv->ReleaseIntArrayElements(array, pixels, 0);


        auto result = std::make_shared<GLTexture>((char*)pixels, width, height);
        return result;
    }
};

std::shared_ptr<GLWorldView> gWorldView;
std::shared_ptr<Ctrl> flappyCtrl;

void preinit() {
    if (flappyCtrl != nullptr)
        return;
    LOGI("preinit");
    pthread_mutex_init(&lock, NULL);
    flappyCtrl = std::make_shared<Ctrl>();
    flappyCtrl->init();
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_step(JNIEnv * env, jclass obj);
    JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_click(JNIEnv * env, jobject obj, jint x, jint y);
};

JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height) {
    LOGI("init");
    preinit();
    gWorldView = std::make_shared<GLWorldView>(std::make_shared<GLViewFactoryAndroid>());
    flappyCtrl->setView(gWorldView);
    gWorldView->resize(width, height);
    gWorldView->init();
}

JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_step(JNIEnv * env, jclass jClass) {
    LOGI("step");
    gEnv = env;
    gClass = jClass;
    pthread_mutex_lock(&lock);
    flappyCtrl->step();
    pthread_mutex_unlock(&lock);
    gWorldView->redrawWorld();
}

JNIEXPORT void JNICALL Java_com_android_{!android.id!}_FlappyJNILib_click(JNIEnv * env, jobject obj, jint x, jint y) {
    LOGI("click");
    pthread_mutex_lock(&lock);
    flappyCtrl->mouseClick(x,y);
    pthread_mutex_unlock(&lock);
}
