/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Functions for loading an image files for textures.
 */


#ifndef __IMAGE_UTIL_H__
#define __IMAGE_UTIL_H__

#include "glUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct demoImageRec
{
	GLubyte* data;
	
	GLsizei size;
	
	GLuint width;
	GLuint height;
	GLenum format;
	GLenum type;
	
	GLuint rowByteSize;
	
} demoImage;

demoImage* imgLoadImage(const char* filepathname, int flipVertical);

demoImage* getImgByPath(const char* name);
    
void imgDestroyImage(demoImage* image);

#ifdef __cplusplus
}
#endif

#endif //__IMAGE_UTIL_H__

