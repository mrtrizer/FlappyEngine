#include <memory>

#include <Sdl2Manager.h>
#include <Entity.h>
#include <AppManager.h>
#include <SceneManager.h>
#include <GLViewManager.h>
#include <GLRenderElementFactory.h>
#include <TransformComponent.h>
#include <ScreenManager.h>
#include <ResManager.h>
#include <TextureRes.h>
#include <StdFileMonitorManager.h>
#include <ResRepositoryManager.h>
#include <Sdl2RgbaBitmapResFactory.h>
#include <StdFileLoadManager.h>
#include <DefaultResFactory.h>
#include <GLShaderResFactory.h>
#include <IRgbaBitmapRes.h>
#include <GLTextureRes.h>
#include <FileResFactory.h>
#include <DesktopThread.h>
#include <PosixApplication.h>
#include <GLTextureResFactory.h>
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
                        rootEntity->createComponent<FileResFactory>();
                        rootEntity->createComponent<ResManager<TextRes>> ();
                        rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
                        rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
                        rootEntity->createComponent<GLTextureResFactory>();
                        rootEntity->createComponent<ResManager<TextureRes>> ();
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

                        // English left
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-300.0f, 300.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("The quick brown fox jumps over the lazy dog");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::LEFT);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(30);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // English center
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-90.0f, 300.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("The quick brown fox jumps over the lazy dog");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::CENTER);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(30);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // English right
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({300.0f - 170.0f, 300.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("The quick brown fox jumps over the lazy dog");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::RIGHT);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(30);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }


                        // Russian left
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-300.0f, 100.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("Съешь ещё этих мягких французских булок, да выпей чаю");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::LEFT);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(20);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // Russian center
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-90.0f, 100.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("Съешь ещё этих мягких французских булок, да выпей чаю");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::CENTER);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(20);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // Russian right
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({300.0f - 170.0f, 100.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("Съешь ещё этих мягких французских булок, да выпей чаю");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::RIGHT);
                            textEntity->component<TextComponent>()->setMaxWidth(170);
                            textEntity->component<TextComponent>()->setSize(20);
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // Numbers and symbols
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-300.0f, -140.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("1234567890+-=*()[]{}");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::LEFT);
                            textEntity->component<TextComponent>()->setSize(20);
                            textEntity->component<TextComponent>()->setColor(Color::fromHex(0x00FF00FF));
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                        // Line break
                        {
                            auto textEntity = sceneEntity->createEntity();
                            textEntity->component<TransformComponent>()->setPos({-300.0f, -200.0f, 0.0f});
                            textEntity->component<TextComponent>()->setText("Hello,\nworld!");
                            textEntity->component<TextComponent>()->setAlign(TextComponent::Align::LEFT);
                            textEntity->component<TextComponent>()->setSize(20);
                            textEntity->component<TextComponent>()->setColor(Color::fromHex(0xFF0000FF));
                            auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                            textEntity->component<TextComponent>()->setFontRes(fontRes);
                        }

                    });
    return application.runThread(currentThread);
}
