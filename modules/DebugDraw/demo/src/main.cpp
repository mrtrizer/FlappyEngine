#include <memory>

#include <Sdl2Manager.h>
#include <Entity.hpp>
#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <BasicLoop.hpp>
#include <BasicLoopManager.hpp>
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
#include <TextResFactory.h>
#include <GLTextureResFactory.h>
#include <AtlasResFactory.h>
#include <TextComponent.h>
#include <GlyphSheetRes.h>
#include <GlyphSheetResFactory.h>
#include <FontRes.h>
#include <FontResFactory.h>
#include <RenderUtils.h>
#include <MaterialResFactory.h>
#include <DebugDrawManager.h>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    const glm::uvec2 screenSize {600, 600};
    
    // Sdl2 and render
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    auto rootEntity = hierarchy->rootEntity();
    hierarchy->initManager<UpdateManager>();
    auto screenManager = hierarchy->initManager<ScreenManager>();
    screenManager->resize(screenSize);
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto repoManager = hierarchy->initManager<ResRepositoryManager>();
    repoManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    hierarchy->initManager<ResManager<TextRes>> ();
    hierarchy->initManager<ResFactory<JsonRes>, DefaultResFactory<JsonRes, JsonRes, TextRes>>();
    hierarchy->initManager<ResManager<JsonRes>>();
    hierarchy->initManager<ResFactory<AtlasRes>, AtlasResFactory>();
    hierarchy->initManager<ResManager<AtlasRes>> ();
    hierarchy->initManager<ResFactory<IRgbaBitmapRes>, Sdl2RgbaBitmapResFactory> ();
    hierarchy->initManager<ResManager<IRgbaBitmapRes>> ();
    hierarchy->initManager<ResFactory<TextureRes>, GLTextureResFactory>();
    hierarchy->initManager<ResManager<TextureRes>> ();
    hierarchy->initManager<ResFactory<SpriteRes>, SpriteResFactory>();
    auto spriteResManager = hierarchy->initManager<ResManager<SpriteRes>> ();
    hierarchy->initManager<ResFactory<ShaderRes>, GLShaderResFactory> ();
    hierarchy->initManager<ResManager<ShaderRes>> ();
    hierarchy->initManager<ResFactory<GlyphSheetRes>, GlyphSheetResFactory> ();
    hierarchy->initManager<ResManager<GlyphSheetRes>> ();
    hierarchy->initManager<ResFactory<FontRes>, FontResFactory> ();
    hierarchy->initManager<ResFactory<MaterialRes>, MaterialResFactory> ();
    auto materialManager = hierarchy->initManager<ResManager<MaterialRes>> ();
    auto fontResManager = hierarchy->initManager<ResManager<FontRes>>();
    auto sceneManager = hierarchy->initManager<SceneManager>();
    auto basicLoopManager = hierarchy->initManager<BasicLoopManager>();
    auto sdl2Manager = hierarchy->initManager<IGLManager, Sdl2Manager>();
    hierarchy->initManager<RenderManager, GLRenderManager>();
    hierarchy->initManager<RenderElementFactory, GLRenderElementFactory>();
    auto debugDrawManager = hierarchy->initManager<DebugDrawManager>();

    // Scene
    sceneManager->setMainCamera(rootEntity->component<CameraComponent>());
    rootEntity->component<CameraComponent>()->setSize(screenSize);

    debugDrawManager->drawCircle({0.0f, 0.0f, 0.0f}, 50.0f, 10.0f, "test1");
    debugDrawManager->drawRect({-10.0f, -10.0f, 20.0f, 20.0f}, 5.0f, "test2");

    return BasicLoop(30, basicLoopManager).run([&hierarchy, &sdl2Manager](float dt) {
        hierarchy->manager<UpdateManager>()->update(dt);
        sdl2Manager->update(dt);
    });
//
//    PosixApplication application;
//    auto currentThread = std::make_shared<DesktopThread>(
//                [argc, argv](SafePtr<Entity> rootEntity) {
//
//                        // Sdl2 and render
//                        auto sdl2Manager = rootEntity->createComponent<Sdl2Manager>();
//                        rootEntity->createComponent<ScreenManager>(600, 600);
//                        rootEntity->createComponent<AppManager>(argc, argv);
//                        rootEntity->createComponent<ResRepositoryManager>("./resources");
//                        rootEntity->createComponent<StdFileMonitorManager>();
//                        rootEntity->createComponent<StdFileLoadManager>();
//                        rootEntity->createComponent<DefaultResFactory<JsonRes, JsonRes, TextRes>>();
//                        rootEntity->createComponent<ResManager<JsonRes>>();
//                        rootEntity->createComponent<AtlasResFactory>();
//                        rootEntity->createComponent<ResManager<AtlasRes>> ();
//                        rootEntity->createComponent<TextResFactory>();
//                        rootEntity->createComponent<ResManager<TextRes>> ();
//                        rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
//                        rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
//                        rootEntity->createComponent<GLTextureResFactory>();
//                        rootEntity->createComponent<ResManager<TextureRes>> ();
//                        rootEntity->createComponent<SpriteResFactory>();
//                        rootEntity->createComponent<ResManager<SpriteRes>> ();
//                        rootEntity->createComponent<GLShaderResFactory> ();
//                        rootEntity->createComponent<ResManager<ShaderRes>> ();
//                        rootEntity->createComponent<GlyphSheetResFactory> ();
//                        rootEntity->createComponent<ResManager<GlyphSheetRes>> ();
//                        rootEntity->createComponent<FontResFactory> ();
//                        rootEntity->createComponent<MaterialResFactory> ();
//                        rootEntity->createComponent<ResManager<MaterialRes>> ();
//                        rootEntity->createComponent<ResManager<FontRes>>();
//
//                        // Scene
//                        auto sceneEntity = rootEntity->createEntity();
//                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
//                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
//                        sceneEntity->component<GLRenderManager>();
//                        sceneEntity->component<GLRenderElementFactory>();
//                        auto debugDrawManager = sceneEntity->createComponent<DebugDrawManager>();
//
//                        debugDrawManager->drawCircle({0.0f, 0.0f, 0.0f}, 50.0f, 10.0f, "test1");
//                        debugDrawManager->drawRect({-10.0f, -10.0f, 20.0f, 20.0f}, 5.0f, "test2");
//
//                    });
//    return application.runThread(currentThread);
}
