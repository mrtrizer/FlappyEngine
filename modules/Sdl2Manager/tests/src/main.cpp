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
#include <StdFileMonitor.h>
#include <SpriteComponent.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();

    // Sdl2 and render

    rootEntity->createComponent<ScreenManager>(600, 600);
    rootEntity->createComponent<AppManager>(argc, argv);
    auto resManager = rootEntity->createComponent<ResManager>();
    auto stdFileMonitor = std::make_shared<StdFileMonitor>();
    resManager->bindResFactory<TextureRes>(std::make_shared<Sdl2GlTextureResFactory>("./resources"));
    resManager->bindResFactory<QuadRes>(std::make_shared<QuadResFactory>());



    // Scene
    auto sceneEntity = rootEntity->createEntity();
    sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
    sceneEntity->component<CameraComponent>()->setSize({600, 600});
    sceneEntity->component<GLViewManager>();
    sceneEntity->component<GLRenderElementFactory>();



    // Some rect
    auto rectEntity = sceneEntity->createEntity();
    rectEntity->component<MeshComponent>();
    rectEntity->component<TransformComponent>()->setScale({10.0f, 10.0f});



    // Sprite
    auto spriteEntity = sceneEntity->createEntity();
    spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
    spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});

    rootEntity->createComponent<Sdl2Manager>();

    spriteEntity->component<SpriteComponent>()->setQuadRes(resManager->getRes<QuadRes>("__img_missing__"));



    return rootEntity->findComponent<AGLManager>()->startMainLoop();
}
