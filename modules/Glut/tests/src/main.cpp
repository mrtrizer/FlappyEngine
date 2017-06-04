#include <memory>

#include <GlutManager.h>
#include <Entity.h>
#include <AppManager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();
    auto glutManager = std::make_shared<GlutManager>();

    rootEntity->addComponent(glutManager);
    rootEntity->addComponent(std::make_shared<AppManager>(argc, argv));
    glutManager->startMainLoop();
    return 0;
}
