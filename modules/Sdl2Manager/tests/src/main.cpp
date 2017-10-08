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
#include <DesktopThread.h>
#include <PosixApplication.h>
#include <GLTextureResFactory.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    PosixApplication application;
    auto currentThread = std::make_shared<DesktopThread>(
                [argc, argv](SafePtr<Entity> rootEntity) {

                        // Sdl2 and render
                        auto sdl2Manager = rootEntity->createComponent<Sdl2Manager>();
                        rootEntity->createComponent<ScreenManager>(600, 600);
                        rootEntity->createComponent<AppManager>(argc, argv);
                        rootEntity->createComponent<ResRepositoryManager>("./resources");
                        rootEntity->createComponent<StdFileMonitorManager>();
                        rootEntity->createComponent<StdFileLoadManager>();
                        rootEntity->createComponent<DefaultResFactory<AtlasRes, AtlasRes>>();
                        rootEntity->createComponent<ResManager<AtlasRes>> ();
                        rootEntity->createComponent<FileResFactory>();
                        rootEntity->createComponent<ResManager<TextRes>> ();
                        rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
                        rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
                        rootEntity->createComponent<GLTextureResFactory>();
                        rootEntity->createComponent<ResManager<TextureRes>> ();
                        rootEntity->createComponent<SpriteResFactory>();
                        rootEntity->createComponent<ResManager<SpriteRes>> ();
                        rootEntity->createComponent<GLShaderResFactory> ();
                        rootEntity->createComponent<ResManager<GLShaderRes>> ();

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
                        sceneEntity->component<GLViewManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Sprite
                        auto spriteEntity = sceneEntity->createEntity();
                        spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
                        spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});
                        auto quadRes = rootEntity->component<ResManager<SpriteRes>>()->getRes("__img_missing__", ExecType::ASYNC);
                        spriteEntity->component<SpriteComponent>()->setSpriteRes(quadRes);

                        // Dynamic box
                        auto rectEntity = sceneEntity->createEntity();
                        rectEntity->component<MeshComponent>();
                        rectEntity->component<TransformComponent>()->setScale({10.0f, 10.0f});
                        rectEntity->component<TransformComponent>()->setPos({0.0f, 4.0f, 0.0f});
                        rectEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);

                        // Try to reinitialize gl context several times
                        rootEntity->removeComponent(sdl2Manager);
                        sdl2Manager = rootEntity->createComponent<Sdl2Manager>();
                        rootEntity->removeComponent(sdl2Manager);
                        sdl2Manager = rootEntity->createComponent<Sdl2Manager>();
                        sdl2Manager->setActive(false);
                        sdl2Manager->setActive(true);
                    });
    return application.runThread(currentThread);
}
