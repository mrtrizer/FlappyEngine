#include <memory>

#include <glut/glutmgr.h>
#include <qt/qtresourcemgr.h>
#include <gl/glviewfactory.h>
#include <core/flappyapp.h>

int main(int argc, char *argv[])
{
    FlappyApp::inst().setResourceMgr(std::make_shared<QtResourceMgr>(":///{?name.lower()?}/res/"));
    GLUTMgr::initGLUT(argc, argv, std::make_shared<GLViewFactory>());
    return 0;
}
