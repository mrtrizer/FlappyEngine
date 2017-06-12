#include <memory>

#include <GlutManager.h>
#include <Entity.h>
#include <AppManager.h>
#include <SceneManager.h>
#include <GLViewManager.h>
#include <GLRenderElementFactory.h>
#include <TransformComponent.h>
#include <ScreenManager.h>
#include <MeshComponent.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();

    // Glut and render
    rootEntity->createComponent<GLViewManager>();
    rootEntity->createComponent<GlutManager>();
    rootEntity->createComponent<ScreenManager>(600, 600);
    rootEntity->createComponent<SceneManager>();
    rootEntity->createComponent<AppManager>(argc, argv);
    rootEntity->component<CameraComponent>()->setSize({600, 600});
    rootEntity->component<SceneManager>()->setMainCamera(rootEntity->component<CameraComponent>());

    // Scene
    auto sceneEntity = rootEntity->createEntity();
    sceneEntity->createComponent<SceneManager>();
    auto camera = sceneEntity->createEntity();
    camera->createComponent<CameraComponent>();

    // Some rect
    auto rectEntity = sceneEntity->createEntity();
    rectEntity->component<MeshComponent>();
    rectEntity->component<TransformComponent>()->setScale({10.0f, 10.0f});

    rootEntity->createComponent<GLRenderElementFactory>();

    return rootEntity->findComponent<AGLManager>()->startMainLoop();
}
