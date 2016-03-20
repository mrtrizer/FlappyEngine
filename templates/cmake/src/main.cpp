#include <ctrl.h>
#include <glut/glutmgr.h>
#ifdef PNG_FOUND
#include <png/viewfactorylibpng.h>
#elif SDL_IMAGE_FOUND
#include <sdl/viewfactorysdl.h>
#endif
#include <iostream>

int main(int argc, char *argv[])
{
    #ifdef PNG_FOUND
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactoryLibPNG>("./res/"), std::make_shared<Ctrl>());
    #elif SDL_IMAGE_FOUND
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactorySDL>("./res/"), std::make_shared<Ctrl>());
    #endif
    return 0;
}
