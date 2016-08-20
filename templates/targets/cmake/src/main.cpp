#include <memory>

{?IF not console_mode?}
#include <glut/glutmgr.h>
#ifdef PNG_FOUND
#include <png/libpngresourceloader.h>
#elif SDL_IMAGE_FOUND
#include <sdl/sdlresourceloader.h>
#endif
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
    auto flappyApp = make_shared<FlappyApp>();
{?IF not console_mode?}
#ifdef PNG_FOUND
    flappyApp->createMgr<ResourceMgr>(make_shared<LibPNGResourceLoader>("./res/"));
#elif SDL_IMAGE_FOUND
    flappyApp->createMgr<ResourceMgr>(make_shared<SDLResourceLoader>("./res/"));
#endif
    flappyApp->createMgr<EntityMgr>();
    flappyApp->createMgr<SceneMgr>();
    flappyApp->createMgr<ScreenMgr>();
    flappyApp->createMgr<InputMgr>();
    GLUTMgr::initGLUT(argc, argv, flappyApp);
{?ENDIF?}
    flappyApp->createMgr<game::MyGameMgr>();

    flappyApp->init();

{?IF not console_mode?}
    return GLUTMgr::mainLoop();
{?ENDIF?}
{?IF console_mode?}
    return 0;
{?ENDIF?}
}
