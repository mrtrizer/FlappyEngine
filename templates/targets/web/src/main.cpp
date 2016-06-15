#include <ctrl.h>
#include <glut/glutmgr.h>
#include <sdl/viewfactorysdl.h>
#include <iostream>

int main(int argc, char *argv[])
{
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactorySDL>("./res/"), std::make_shared<Ctrl>());
    return 0;
}
