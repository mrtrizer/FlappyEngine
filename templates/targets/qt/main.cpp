#include <memory>

#include <glut/glutmgr.h>
#include <qt/viewfactoryqt.h>

#include <ctrl.h>

int main(int argc, char *argv[])
{
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactoryQt>(":///{?name.lower()?}/res/"), std::make_shared<Ctrl>());
    return 0;
}
