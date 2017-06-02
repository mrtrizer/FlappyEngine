#include <memory>

#include <GlutManager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();
    auto glutManager = std::make_shared<GlutManager>();

    rootEntity->addComponent(glutManager);
    return 0;
}
