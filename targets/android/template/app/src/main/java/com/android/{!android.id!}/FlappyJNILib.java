/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.{!android.id!};

// Wrapper for native library

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;

public class FlappyJNILib {

     static {
         System.loadLibrary("gnustl_shared");
         System.loadLibrary("{!android.id!}");
     }

    public static native void init(int width, int height);
    public static native void step();
    public static native void click(int x, int y);
    public static Bitmap loadBitmap(String path) {
        try {
            //return BitmapFactory.decodeStream(amgr.open(path));
            //return BitmapFactory.decodeResource(amgr,id);
            return BitmapFactory.decodeResource(amgr,amgr.getIdentifier(path,"drawable",packageName));
        } catch (Exception e) { }
        Bitmap bitmap = Bitmap.createBitmap(64,64, Bitmap.Config.ARGB_8888);
        bitmap.setPixel(10,10, Color.RED);
        bitmap.setPixel(10,11, Color.RED);
        bitmap.setPixel(10,12, Color.RED);
        return bitmap;
    }

    public static Resources amgr;
    public static String packageName;
}
