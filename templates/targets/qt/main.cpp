#include <memory>

{?IF not console_mode?}
#include <glut/glutmgr.h>
#include <qt/qtresourceloader.h>
#include <gl/glviewfactory.h>
#include <core/resourcemgr.h>
#include <core/inputmgr.h>
#include <core/entitymgr.h>
#include <core/scenemgr.h>
#include <core/screenmgr.h>
{?ENDIF?}
#include <core/flappyapp.h>
#include <mygamemgr.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto flappyApp = make_shared<FlappyApp>(argc, const_cast<const char **>(argv));
{?IF not console_mode?}
    flappyApp->createMgr<ResourceMgr>(make_shared<QtResourceLoader>(":///{?name.lower()?}/res/"));
    flappyApp->createMgr<EntityMgr>();
    flappyApp->createMgr<SceneMgr>();
    flappyApp->createMgr<ScreenMgr>();
    flappyApp->createMgr<InputMgr>();
    GLUTMgr::initGLUT(argc, argv, flappyApp);
{?ENDIF?}
{?IF console_mode?}
    flappyApp->createMgr<game::MyGameMgr>();
{?ENDIF?}

    flappyApp->init();

{?IF not console_mode?}
    flappyApp->MGR<SceneMgr>()->setScene(make_shared<game::MyScene>());

    return GLUTMgr::mainLoop();
{?ENDIF?}
{?IF console_mode?}
    return 0;
{?ENDIF?}
}
