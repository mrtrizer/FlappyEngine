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
#include <Box2DWorldManager.h>
#include <Box2DBodyComponent.h>
#include <Box2DFixtureComponent.h>

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
                        rootEntity->createComponent<ResManager<AtlasRes>> ();
                        rootEntity->createComponent<FileResFactory<TextRes>>();
                        rootEntity->createComponent<ResManager<TextRes>> ();
                        rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
                        rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
                        rootEntity->createComponent<GLTextureResFactory>();
                        rootEntity->createComponent<ResManager<TextureRes>> ();
                        rootEntity->createComponent<SpriteResFactory>();
                        rootEntity->createComponent<ResManager<SpriteRes>> ();
                        rootEntity->createComponent<GLShaderResFactory> ();
                        rootEntity->createComponent<ResManager<GLShaderRes>> ();
                        rootEntity->createComponent<DefaultResFactory<AtlasRes, AtlasRes>>();

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
                        sceneEntity->component<GLViewManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Sprite
                        auto spriteEntity = sceneEntity->createEntity();
                        spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
                        spriteEntity->component<TransformComponent>()->setPos({200.0f, 1.0f, 1.0f});
                        auto quadRes = rootEntity->component<ResManager<SpriteRes>>()->getRes("__img_missing__");
                        spriteEntity->component<SpriteComponent>()->setSpriteRes(quadRes);

                        // Dynamic box
                        auto rectEntity = sceneEntity->createEntity();
                        rectEntity->component<MeshComponent>();
                        rectEntity->component<TransformComponent>()->setScale({10.0f, 10.0f});
                        rectEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
                        rectEntity->component<TransformComponent>()->setPos({0.0f, 4.0f, 0.0f});
                        rectEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
                        rectEntity->component<Box2DFixtureComponent>()->setDensity(10.0f);
                        rectEntity->component<Box2DFixtureComponent>()->setFriction(0.3f);
                        auto dynamicBox = std::make_shared<b2PolygonShape>();
                        dynamicBox->SetAsBox(1.0f, 1.0f);
                        rectEntity->component<Box2DFixtureComponent>()->setShape(dynamicBox);

                        // Box2D ground
                        auto groundEntity = sceneEntity->createEntity();
                        groundEntity->component<MeshComponent>();
                        groundEntity->component<TransformComponent>()->setScale({100.0f, 10.0f});
                        groundEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.25f);
                        groundEntity->component<Box2DBodyComponent>()->setType(b2_staticBody);
                        groundEntity->component<TransformComponent>()->setPos({0.0f, -100.0f, 0.0f});
                        auto groundBox = std::make_shared<b2PolygonShape>();
                        groundBox->SetAsBox(100.0f, 10.0f);
                        groundEntity->component<Box2DFixtureComponent>()->setShape(groundBox);

                        // Box2D world
                        rootEntity->component<Box2DWorldManager>()->setVelocityIterations(6);
                        rootEntity->component<Box2DWorldManager>()->setPositionIterations(2);
                        rootEntity->component<Box2DWorldManager>()->setGravity({0.0f, -10.0f});
                        rootEntity->component<Box2DWorldManager>()->setSizeFactor(2.0f);

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
