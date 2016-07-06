//
//  GLWrapper.cpp
//  GLEssentials
//
//  Created by Denis Zdorovtsov on 24.06.16.
//  Copyright © 2016 Dan Omachi. All rights reserved.
//

#include "GLWrapper.hpp"
#include <gl/glattribarray.h>
#include <gl/glviewshape.h>
#include <OpenGLES/ES2/gl.h>
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gl/glworldview.h>
#include <gl/glviewfactory.h>
#include <gl/gltexture.h>
#include "imageUtil.h"

#include <myflappyapp.h>

class IOSViewFactory: public GLViewFactory {
public:
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string path) const {
        auto img = getImgByPath(path.data());
        auto texture = std::make_shared<GLTexture>((char*)img->data, (int)img->width, (int)img->height);
        imgDestroyImage(img);
        return texture;
    }
};

entityx::ComponentHandle<GLWorldView> glWorldView;
std::shared_ptr<FlappyApp> app;

extern "C" {

std::shared_ptr<GLViewShape> myRect;

void renderCPP() {

    if (app != nullptr)
        app->update();

}

void setSizeCPP(int width, int height) {
    GWorldView::instance->resize(width,height);
    //GWorldView::getInst()->resize(width, height);
}
    
void initCPP() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    CHECK_GL_ERROR;
    glEnable (GL_BLEND);
    CHECK_GL_ERROR;
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL_ERROR;
    app = std::make_shared<MyFlappyApp>();
    
    app->systems.add<GLWorldView>(std::make_shared<IOSViewFactory>());
    app->configure();
}
}