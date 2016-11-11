#include <memory>

#include <glut/glutinit.h>
#include <qt/TextureFactoryQt.h>
#include <core/resmanager.h>
#include <core/inputmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>
#include <core/screenmanager.h>
#include <core/atlasresfactory.h>
#include <core/quadresfactory.h>
#include <core/managerlist.h>
#include <core/appmanager.h>

#include <{!scene_header!}>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto managerList = make_shared<ManagerList>();
    managerList->create<AppManager>(argc, argv);
    auto resManager = managerList->create<ResManager>();
    resManager->bind<Texture>(make_shared<TextureFactoryQt>(":///tests_gl/res/"));
    resManager->bind<Atlas>(make_shared<AtlasResFactory>());
    resManager->bind<Quad>(make_shared<QuadResFactory>());
    managerList->create<EntityManager>();
    managerList->create<SceneManager>();
    managerList->create<ScreenManager>();
    managerList->create<InputManager>();

    GLUTInit::initGLUT(managerList);

    managerList->init();

    managerList->MGR<SceneManager>()->setScene(make_shared<{!scene_class!}>());

    return GLUTInit::mainLoop();
}
