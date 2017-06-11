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
    rootEntity->createManager<GLViewManager>();
    rootEntity->createManager<GlutManager>();
    rootEntity->createManager<ScreenManager>(600, 600);
    rootEntity->createManager<AppManager>(argc, argv);


    // Scene
    auto sceneEntity = rootEntity->createEntity();
    sceneEntity->createManager<SceneManager>();
    auto camera = sceneEntity->createEntity();
    camera->createComponent<CameraComponent>();

    // Some rect
    auto rectEntity = sceneEntity->createEntity();
    auto transformComponent = rectEntity->createComponent<TransformComponent>();
    transformComponent->setScale({10.0f, 10.0f});
    auto rectComponent = rectEntity->createComponent<GLViewRect>();

    return rootEntity->findComponent<AGLManager>()->startMainLoop();
}
