#include <memory>

{?IF not console_mode?}
#include <glut/glutmanager.h>
#ifdef PNG_FOUND
#include <png/libpngresourceloader.h>
#elif SDL_IMAGE_FOUND
#include <sdl/sdlresourceloader.h>
#endif
#include <gl/glviewfactory.h>
#include <core/resourcemanager.h>
#include <core/inputmanager.h>
#include <core/entitymanager.h>
#include <core/scenemanager.h>
#include <core/screenmanager.h>
{?ENDIF?}
#include <core/flappyapp.h>
#include <mygamemanager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto flappyApp = make_shared<FlappyApp>();
{?IF not console_mode?}
#ifdef PNG_FOUND
    flappyApp->create<ResManager>(make_shared<LibPNGResourceLoader>("./res/"));
#elif SDL_IMAGE_FOUND
    flappyApp->create<ResManager>(make_shared<SDLResourceLoader>("./res/"));
#endif
    flappyApp->create<EntityManager>();
    flappyApp->create<SceneManager>();
    flappyApp->create<ScreenManager>();
    flappyApp->create<InputManager>();
    GLUTManager::initGLUT(argc, argv, flappyApp);
{?ENDIF?}
    flappyApp->create<game::MyGameManager>();

    flappyApp->init();

{?IF not console_mode?}
    return GLUTManager::mainLoop();
{?ENDIF?}
{?IF console_mode?}
    return 0;
{?ENDIF?}
}
