#include <memory>

#include <glut/glutinit.h>
#include <qt/resources/QtTextureResFactory.h>
#include <managers/ResManager/ResManager.h>
#include <core/inputmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>
#include <core/screenmanager.h>
#include <core/managerlist.h>
#include <core/appmanager.h>
#include <qt/QtFileMonitor.h>
#include <resources/QuadRes.h>
#include <resources/TextureRes.h>
#include <resources/QuadResFactory.h>

#include <{!scene_header!}>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto managerList = make_shared<ManagerList>();
    managerList->create<AppManager>(argc, argv);
    auto qtFileMonitor = std::make_shared<QtFileMonitor>();
    auto resManager = managerList->create<ResManager>();
    resManager->bindResFactory<TextureRes>(make_shared<QtTextureResFactory>("../../res/", qtFileMonitor));
    resManager->bindResFactory<QuadRes>(make_shared<QuadResFactory>());
    managerList->create<EntityManager>();
    managerList->create<SceneManager>();
    managerList->create<ScreenManager>();
    managerList->create<InputManager>();

    GLUTInit::initGLUT(managerList);

    managerList->init();

    managerList->MGR<SceneManager>()->setScene(make_shared<{!scene_class!}>());

    return GLUTInit::mainLoop();
}
