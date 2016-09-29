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

#include <core/resmanager.h>
#include <core/inputmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>
#include <core/screenmanager.h>
#include <core/atlasresfactory.h>
#include <core/quadresfactory.h>
#include <core/managerlist.h>
#include <core/appmanager.h>
#include <core/res.h>
#include <gl/gltexture.h>
#include <gl/glviewmanager.h>
#include <gl/glviewcircle.h>
#include <gl/glviewrect.h>
#include <gl/glviewsprite.h>
#include <mygamemanager.h>
#include "imageUtil.h"

using namespace flappy;
using namespace std;

//class IOSTextureResFactory: public GLViewFactory {
//public:
//    virtual std::shared_ptr<GLTexture> getGLTexture(std::string path) const {
//        auto img = getImgByPath(path.data());
//        auto texture = std::make_shared<GLTexture>((char*)img->data, (int)img->width, (int)img->height);
//        imgDestroyImage(img);
//        return texture;
//    }
//};

class IOSTextureResFactory: public IResFactory {
public:
    std::shared_ptr<IRes> load(const std::string& path) override {
        auto img = getImgByPath(path.data());
        auto texture = std::make_shared<GLTexture>((char*)img->data, (int)img->width, (int)img->height);
        imgDestroyImage(img);
        return texture;
    }
};

//entityx::ComponentHandle<GLWorldView> glWorldView;
//std::shared_ptr<FlappyApp> app;
std::shared_ptr<ManagerList> managerList;

namespace flappy {
  class CircleShape;
  class RectShape;
  class Sprite;
}

extern "C" {

    //std::shared_ptr<GLViewShape> myRect;

    void renderCPP() {
        if (managerList != nullptr)
            managerList->update();
    }

    void setSizeCPP(int width, int height) {
        managerList->MGR<ViewManager>()->resize(width,height);
        //GWorldView::getInst()->resize(width, height);
    }

    void initCPP() {
        managerList = make_shared<ManagerList>();
        managerList->create<AppManager>();
        auto resManager = managerList->create<ResManager>();
        resManager->bind<Texture>(make_shared<IOSTextureResFactory>());
        resManager->bind<Atlas>(make_shared<AtlasResFactory>());
        resManager->bind<Quad>(make_shared<QuadResFactory>());
        managerList->create<EntityManager>();
        managerList->create<SceneManager>();
        managerList->create<ScreenManager>();
        managerList->create<InputManager>();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        CHECK_GL_ERROR;
        glEnable (GL_BLEND);
        CHECK_GL_ERROR;
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CHECK_GL_ERROR;

        auto manager = managerList->override<ViewManager, GLViewManager>();
        manager->bind<CircleShape,GLViewCircle>();
        manager->bind<RectShape,GLViewRect>();
        manager->bind<Sprite,GLViewSprite>();

        managerList->init();

        managerList->MGR<SceneManager>()->setScene(make_shared<game::MyScene>());

    }
}
