#include <memory>

#include <glut/glutmgr.h>
#include <qt/qtresourcemgr.h>
#include <gl/glviewfactory.h>
#include <core/flappyapp.h>
#include <core/inputmgr.h>
#include <core/entitymgr.h>
#include <core/scenemgr.h>
#include <core/screenmgr.h>
#include <mygamemgr.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto flappyApp = make_shared<FlappyApp>();

    flappyApp->createMgr<QtResourceMgr>(":///{?name.lower()?}/res/");
    flappyApp->createMgr<game::MyGameMgr>();
    flappyApp->createMgr<EntityMgr>();
    flappyApp->createMgr<SceneMgr>();
    flappyApp->createMgr<ScreenMgr>();
    flappyApp->createMgr<InputMgr>();

    GLUTMgr::initGLUT(argc, argv, flappyApp);

    flappyApp->init();

    return GLUTMgr::mainLoop();
}
