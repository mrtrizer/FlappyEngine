#include <memory>

#include <Sdl2Manager.h>
#include <Entity.h>
#include <AppManager.h>
#include <SceneManager.h>
#include <GLViewManager.h>
#include <GLRenderElementFactory.h>
#include <TransformComponent.h>
#include <ScreenManager.h>
#include <MeshComponent.h>
#include <ResManager.h>
#include <QuadRes.h>
#include <QuadResFactory.h>
#include <TextureRes.h>
#include <Sdl2GlTextureResFactory.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();

    // Sdl2 and render
    rootEntity->createComponent<Sdl2Manager>();
    rootEntity->createComponent<ScreenManager>(600, 600);
    rootEntity->createComponent<AppManager>(argc, argv);

    // Scene
    auto sceneEntity = rootEntity->createEntity();
    sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
    sceneEntity->component<CameraComponent>()->setSize({600,600});
    sceneEntity->component<GLViewManager>();
    sceneEntity->component<GLRenderElementFactory>();
    sceneEntity->component<ResManager>()->bindResFactory<QuadRes>(std::make_shared<QuadResFactory>());
    sceneEntity->component<ResManager>()->bindResFactory<TextureRes>(std::make_shared<Sdl2GlTextureResFactory>("."));

    // Some rect
    auto rectEntity = sceneEntity->createEntity();
    rectEntity->component<MeshComponent>();
    rectEntity->component<TransformComponent>()->setScale({10.0f, 10.0f});

    return rootEntity->findComponent<AGLManager>()->startMainLoop();
}
