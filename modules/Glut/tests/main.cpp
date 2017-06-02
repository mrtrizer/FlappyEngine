#include <memory>

#include <GlutModule>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();
    auto testComponent = std::make_shared<Component>();

    rootEntity->addComponent(testComponent);
    return GLUTInit::mainLoop();
}
