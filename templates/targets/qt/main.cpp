#include <memory>

{?IF not console_mode?}
#include <glut/glutinit.h>
#include <qt/qttextureresfactory.h>
#include <core/resmanager.h>
#include <core/inputmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>
#include <core/screenmanager.h>
#include <core/atlasresfactory.h>
#include <core/quadresfactory.h>
{?ENDIF?}
#include <core/managerlist.h>
#include <core/appmanager.h>
#include <mygamemanager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto managerList = make_shared<ManagerList>();
    managerList->create<AppManager>(argc, argv);
{?IF not console_mode?}
    auto resManager = managerList->create<ResManager>();
    resManager->bind<Texture>(make_shared<QtTextureResFactory>(":///tests_gl/res/"));
    resManager->bind<Atlas>(make_shared<AtlasResFactory>());
    resManager->bind<Quad>(make_shared<QuadResFactory>());
    managerList->create<EntityManager>();
    managerList->create<SceneManager>();
    managerList->create<ScreenManager>();
    managerList->create<InputManager>();

    GLUTInit::initGLUT(managerList);
{?ENDIF?}
{?IF console_mode?}
    managerList->create<game::MyGameManager>();
{?ENDIF?}

    managerList->init();

{?IF not console_mode?}
    managerList->MGR<SceneManager>()->setScene(make_shared<game::MyScene>());

    return GLUTInit::mainLoop();
{?ENDIF?}
{?IF console_mode?}
    return 0;
{?ENDIF?}
}
