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

#include <core/scenemanager.h>
#include <core/cpresenter.h>
#include <core/ctransform.h>
#include <core/cbehavoiur.h>
#include <core/animation.h>
#include <core/flappyapp.h>

#include <gl/glworldview.h>
#include <gl/glviewfactory.h>
#include <gl/gltexture.h>

class IOSViewFactory: public GLViewFactory {
public:
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string path) const {
        return std::make_shared<GLTexture>(nullptr,100,100);
    }
};

entityx::ComponentHandle<GLWorldView> glWorldView;
std::shared_ptr<FlappyApp> app;

class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        SceneManager::createEntity().add<CCamera>();
        //Background
        auto sprite = SceneManager::createEntity();
        sprite.add<CPresenter,GPresenterRect>(10, 10);
    }
};


extern "C" {

std::shared_ptr<GLViewShape> myRect;

void renderCPP() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glm::mat4 pMatrix = glm::ortho( -100.f, 100.f, -100.f, 100.f, -1.f, 1.f );
//    glm::mat4 mvMatrix = glm::mat4(1);
    
//    if (myRect != nullptr)
//        myRect->draw(pMatrix, mvMatrix);
    if (app != nullptr)
        app->update();

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