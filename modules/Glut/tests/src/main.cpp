#include <memory>

#include <GlutManager.h>
#include <Entity.h>
#include <AppManager.h>
#include <SceneManager.h>
#include <GLViewManager.h>
#include <TransformComponent.h>
#include <GLViewRect.h>
#include <ScreenManager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();

    // Glut and render
    auto glutManager = rootEntity->createManager<GlutManager>();
    rootEntity->createManager<AppManager>(argc, argv);
    rootEntity->createManager<ScreenManager>();
    rootEntity->createManager<GLViewManager>();

    // Scene
    auto sceneEntity = rootEntity->createEntity();
    sceneEntity->createManager<SceneManager>();
    auto camera = sceneEntity->createEntity();
    camera->createComponent<CameraComponent>();

    // Some rect
    auto rectEntity = sceneEntity->createEntity();
    auto transformComponent = rectEntity->createComponent<TransformComponent>();
    auto rectComponent = rectEntity->createComponent<GLViewRect>();

    return glutManager->startMainLoop();
}
