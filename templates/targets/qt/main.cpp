#include <memory>

{?IF not console_mode?}
#include <glut/glutinit.h>
#include <qt/qtresourceloader.h>
#include <gl/glviewfactory.h>
#include <core/resourcemgr.h>
#include <core/inputmgr.h>
#include <core/entitymgr.h>
#include <core/scenemgr.h>
#include <core/screenmgr.h>
{?ENDIF?}
#include <core/managerlist.h>
#include <core/appmgr.h>
#include <mygamemgr.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto managerList = make_shared<ManagerList>();
    managerList->createMgr<AppMgr>(argc, argv);
{?IF not console_mode?}
    managerList->createMgr<ResourceMgr>(make_shared<QtResourceLoader>(":///{?name.lower()?}/res/"));
    managerList->createMgr<EntityMgr>();
    managerList->createMgr<SceneMgr>();
    managerList->createMgr<ScreenMgr>();
    managerList->createMgr<InputMgr>();


    GLUTInit::initGLUT(managerList);
{?ENDIF?}
{?IF console_mode?}
    managerList->createMgr<game::MyGameMgr>();
{?ENDIF?}

    managerList->init();

{?IF not console_mode?}
    managerList->MGR<SceneMgr>()->setScene(make_shared<game::MyScene>());

    return GLUTInit::mainLoop();
{?ENDIF?}
{?IF console_mode?}
    return 0;
{?ENDIF?}
}
