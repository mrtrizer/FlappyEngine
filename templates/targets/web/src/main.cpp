#include <ctrl.h>
#include <glut/glutmanager.h>
#include <sdl/viewfactorysdl.h>
#include <iostream>

int main(int argc, char *argv[])
{
    GLUTManager::initGLUT(argc, argv, std::make_shared<ViewFactorySDL>("./res/"), std::make_shared<Ctrl>());
    return 0;
}
