#include <memory>

#include <glut/glutmgr.h>
#include <qt/viewfactoryqt.h>
#include <core/flappyapp.h>

int main(int argc, char *argv[])
{
    GLUTMgr::initGLUT(argc, argv, std::make_shared<ViewFactoryQt>(":///{?name.lower()?}/res/"));
    return 0;
}
