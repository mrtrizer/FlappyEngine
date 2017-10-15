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
#include <StdFileMonitorManager.h>
#include <ResRepositoryManager.h>
#include <StdFileLoadManager.h>
#include <DefaultResFactory.h>
#include <GLShaderResFactory.h>
#include <FileResFactory.h>
#include <DesktopThread.h>
#include <PosixApplication.h>
#include <GLTextureResFactory.h>
#include <Box2DWorldManager.h>
#include <Box2DBodyComponent.h>
#include <Box2DBoxComponent.h>
#include <Box2DCircleComponent.h>
#include <RenderUtils.h>

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
                        rootEntity->createComponent<FileResFactory>();
                        rootEntity->createComponent<ResManager<TextRes>> ();
                        rootEntity->createComponent<GLShaderResFactory> ();
                        rootEntity->createComponent<ResManager<GLShaderRes>> ();

                        // Box2D world
                        rootEntity->component<Box2DWorldManager>()->setVelocityIterations(6);
                        rootEntity->component<Box2DWorldManager>()->setPositionIterations(2);
                        rootEntity->component<Box2DWorldManager>()->setGravity({0.0f, -10.0f});
                        rootEntity->component<Box2DWorldManager>()->setSizeFactor(0.1f);

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({300, 300});
                        sceneEntity->component<GLViewManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Dynamic box
                        auto rectEntity = sceneEntity->createEntity();
                        rectEntity->component<MeshComponent>();
                        rectEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                        rectEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
                        rectEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 0.0f});
                        rectEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
                        rectEntity->component<Box2DBoxComponent>()->setDensity(10.0f);
                        rectEntity->component<Box2DBoxComponent>()->setFriction(0.3f);
                        rectEntity->component<Box2DBoxComponent>()->setSize({20.0f, 20.0f});

                        // Dynamic circle
                        auto circleEntity = sceneEntity->createEntity();
                        circleEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
                        circleEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                        circleEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
                        circleEntity->component<TransformComponent>()->setPos({0.0f, 44.0f, 0.0f});
                        circleEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
                        circleEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
                        circleEntity->component<Box2DCircleComponent>()->setFriction(0.3f);
                        circleEntity->component<Box2DCircleComponent>()->setRadius(10.0f);

                        // Box2D ground
                        {
                            auto groundEntity = sceneEntity->createEntity();
                            groundEntity->component<MeshComponent>();
                            groundEntity->component<TransformComponent>()->setScale({100.0f, 10.0f});
                            groundEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.25f);
                            groundEntity->component<Box2DBodyComponent>()->setType(b2_staticBody);
                            groundEntity->component<TransformComponent>()->setPos({0.0f, -100.0f, 0.0f});
                            groundEntity->component<Box2DBoxComponent>()->setSize({100.0f, 10.0f});
                        }

                        {
                            auto groundEntity = sceneEntity->createEntity();
                            groundEntity->component<MeshComponent>();
                            groundEntity->component<TransformComponent>()->setScale({100.0f, 10.0f});
                            groundEntity->component<TransformComponent>()->setAngle2DRad(-M_PI * 0.25f);
                            groundEntity->component<Box2DBodyComponent>()->setType(b2_staticBody);
                            groundEntity->component<TransformComponent>()->setPos({-70.0f, -100.0f, 0.0f});
                            groundEntity->component<Box2DBoxComponent>()->setSize({100.0f, 10.0f});
                        }

                    });
    return application.runThread(currentThread);
}
