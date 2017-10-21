#include <memory>

#include <Sdl2Manager.h>
#include <Entity.h>
#include <AppManager.h>
#include <SceneManager.h>
#include <GLRenderManager.h>
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
#include <RenderUtils.h>
#include <MaterialResFactory.h>

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
                        rootEntity->createComponent<ResManager<ShaderRes>> ();
                        rootEntity->createComponent<GlyphSheetResFactory> ();
                        rootEntity->createComponent<ResManager<GlyphSheetRes>> ();
                        rootEntity->createComponent<FontResFactory> ();
                        rootEntity->createComponent<MaterialResFactory> ();
                        rootEntity->createComponent<ResManager<MaterialRes>> ();
                        rootEntity->createComponent<ResManager<FontRes>>();

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
                        sceneEntity->component<GLRenderManager>();
                        sceneEntity->component<GLRenderElementFactory>();



                    });
    return application.runThread(currentThread);
}
