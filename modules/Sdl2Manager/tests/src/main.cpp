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
#include <SpriteRes.h>
#include <SpriteResFactory.h>
#include <TextureRes.h>
#include <StdFileMonitorManager.h>
#include <ResRepositoryManager.h>
#include <Sdl2RgbaBitmapResFactory.h>
#include <StdFileLoadManager.h>
#include <DefaultResFactory.h>
#include <GLShaderResFactory.h>
#include <IRgbaBitmapRes.h>
#include <GLTextureRes.h>
#include <SpriteComponent.h>
#include <FileResFactory.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();

    // Sdl2 and render
    rootEntity->createComponent<Sdl2Manager>();
    rootEntity->createComponent<ScreenManager>(600, 600);
    rootEntity->createComponent<AppManager>(argc, argv);
    rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    rootEntity->createComponent<ResManager<AtlasRes>> ();
    rootEntity->createComponent<FileResFactory<TextRes>>();
    rootEntity->createComponent<ResManager<TextRes>> ();
    rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
    rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
    rootEntity->createComponent<DefaultResFactory<TextureRes, GLTextureRes, IRgbaBitmapRes>>();
    rootEntity->createComponent<ResManager<TextureRes>> ();
    rootEntity->createComponent<SpriteResFactory>();
    rootEntity->createComponent<ResManager<SpriteRes>> ();
    rootEntity->createComponent<GLShaderResFactory> ();
    rootEntity->createComponent<ResManager<GLShaderProgram>> ();
    rootEntity->createComponent<DefaultResFactory<AtlasRes, AtlasRes>>();

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

//    // Sprite
//    auto spriteEntity = sceneEntity->createEntity();
//    spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
//    spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});


//    auto quadRes = rootEntity->component<ResManager<SpriteRes>>()->getRes("__img_missing__");
//    spriteEntity->component<SpriteComponent>()->setSpriteRes(quadRes);

    return rootEntity->findComponent<AGLManager>()->startMainLoop();
}
