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
#include <AtlasResFactory.h>
#include <TextComponent.h>
#include <GlyphSheetRes.h>
#include <GlyphSheetResFactory.h>
#include <FontRes.h>
#include <FontResFactory.h>

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
                        rootEntity->createComponent<DefaultResFactory<JsonRes, JsonRes, TextRes>>();
                        rootEntity->createComponent<ResManager<JsonRes>>();
                        rootEntity->createComponent<AtlasResFactory>();
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
                        rootEntity->createComponent<GlyphSheetResFactory> ();
                        rootEntity->createComponent<ResManager<GlyphSheetRes>> ();
                        rootEntity->createComponent<FontResFactory> ();
                        auto fontResManager = rootEntity->createComponent<ResManager<FontRes>>();

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
                        sceneEntity->component<GLViewManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Sprite
                        {
                            auto spriteEntity = sceneEntity->createEntity();
                            spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
                            spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});
                            auto spriteResManager = rootEntity->component<ResManager<SpriteRes>>();
                            auto quadRes = spriteResManager->getRes("demo_atlas:blue_button01", ExecType::ASYNC);
                            spriteEntity->component<SpriteComponent>()->setSpriteRes(quadRes);
                        }

                        // Single image sprite
                        {
                            auto spriteEntity = sceneEntity->createEntity();
                            spriteEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 1.0f});
                            auto spriteResManager = rootEntity->component<ResManager<SpriteRes>>();
                            auto quadRes = spriteResManager->getRes("__img_missing__", ExecType::ASYNC);
                            spriteEntity->component<SpriteComponent>()->setSpriteRes(quadRes);
                        }

                        // Shape
                        auto rectEntity = sceneEntity->createEntity();
                        rectEntity->component<MeshComponent>();
                        rectEntity->component<TransformComponent>()->setScale({100.0f, 100.0f});
                        rectEntity->component<TransformComponent>()->setPos({-150.0f, 0.0f, 0.0f});
                        rectEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);

                        // Text
                        auto textEntity = sceneEntity->createEntity();
                        textEntity->component<TransformComponent>()->setPos({-300.0f, 200.0f, 0.0f});
                        textEntity->component<TextComponent>()->setText("AVTest ВАСЯ 123\n456 789");
                        textEntity->component<TextComponent>()->setAlign(TextComponent::Align::CENTER);
                        textEntity->component<TextComponent>()->setMaxWidth(170);
                        textEntity->component<TextComponent>()->setSize(30);
                        auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                        textEntity->component<TextComponent>()->setFontRes(fontRes);


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
