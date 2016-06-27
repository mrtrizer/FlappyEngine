#include <myflappyapp.h>
#include <glut/glutmgr.h>
#ifdef PNG_FOUND
#include <png/viewfactorylibpng.h>
#elif SDL_IMAGE_FOUND
#include <sdl/viewfactorysdl.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef PNG_FOUND
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactoryLibPNG>("./res/"), std::make_shared<MyFlappyApp>());
    #elif SDL_IMAGE_FOUND
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactorySDL>("./res/"), std::make_shared<MyFlappyApp>());
    #endif
    return 0;
}
