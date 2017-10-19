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
#include <Box2DBodyManager.h>
#include <Box2DBoxComponent.h>
#include <Box2DChainComponent.h>
#include <Box2DCircleComponent.h>
#include <RenderUtils.h>
#include <Sdl2MouseInput.h>
#include <MouseInputManager.h>
#include <TextComponent.h>
#include <GlyphSheetResFactory.h>
#include <FontResFactory.h>
#include <Sdl2RgbaBitmapResFactory.h>
#include <Box2DRevoluteJointComponent.h>
#include <KeyboardInputManager.h>
#include <Sdl2KeyboardInput.h>
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
                        rootEntity->createComponent<FileResFactory>();
                        rootEntity->createComponent<ResManager<TextRes>> ();
                        rootEntity->createComponent<GLShaderResFactory> ();
                        rootEntity->createComponent<ResManager<ShaderRes>> ();
                        rootEntity->createComponent<DefaultResFactory<JsonRes, JsonRes, TextRes>>();
                        rootEntity->createComponent<ResManager<JsonRes>>();
                        rootEntity->createComponent<MouseInputManager> ();
                        rootEntity->createComponent<Sdl2MouseInput> ();
                        rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
                        rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
                        rootEntity->createComponent<GLTextureResFactory>();
                        rootEntity->createComponent<ResManager<TextureRes>> ();
                        rootEntity->createComponent<GlyphSheetResFactory> ();
                        rootEntity->createComponent<ResManager<GlyphSheetRes>> ();
                        rootEntity->createComponent<FontResFactory> ();
                        rootEntity->createComponent<Sdl2KeyboardInput> ();
                        rootEntity->createComponent<KeyboardInputManager> ();
                        rootEntity->createComponent<MaterialResFactory> ();
                        rootEntity->createComponent<ResManager<MaterialRes>> ();
                        auto fontResManager = rootEntity->createComponent<ResManager<FontRes>>();

                        // Box2D world
                        rootEntity->component<Box2DWorldManager>()->setVelocityIterations(6);
                        rootEntity->component<Box2DWorldManager>()->setPositionIterations(2);
                        rootEntity->component<Box2DWorldManager>()->setGravity({0.0f, -10.0f});
                        rootEntity->component<Box2DWorldManager>()->setSizeFactor(0.1f);

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({300, 300});
                        sceneEntity->component<GLRenderManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Dynamic box
                        auto rectEntity = sceneEntity->createEntity();
                        rectEntity->component<MeshComponent>();
                        rectEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                        rectEntity->component<Box2DBodyManager>()->setType(b2_dynamicBody);
                        rectEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 0.0f});
                        rectEntity->component<Box2DBoxComponent>()->setDensity(10.0f);
                        rectEntity->component<Box2DBoxComponent>()->setFriction(0.3f);
                        rectEntity->component<Box2DBoxComponent>()->setSize({20.0f, 20.0f});

                        // Dynamic circle
                        auto wheelAEntity = sceneEntity->createEntity();
                        wheelAEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
                        wheelAEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                        wheelAEntity->component<Box2DBodyManager>()->setType(b2_dynamicBody);
                        wheelAEntity->component<TransformComponent>()->setPos({20.0f, -10.0f, 0.0f});
                        wheelAEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
                        wheelAEntity->component<Box2DCircleComponent>()->setFriction(5.0f);
                        wheelAEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setBodyA(wheelAEntity->component<Box2DBodyManager>());
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setBodyB(rectEntity->component<Box2DBodyManager>());
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setLocalAnchorB({20, -10});
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setEnableMotor(true);
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                        wheelAEntity->component<Box2DRevoluteJointComponent>()->setMaxMotorTorque(1000.0f);

                        // Dynamic circle
                        auto wheelBEntity = sceneEntity->createEntity();
                        wheelBEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
                        wheelBEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                        wheelBEntity->component<Box2DBodyManager>()->setType(b2_dynamicBody);
                        wheelBEntity->component<TransformComponent>()->setPos({-20.0f, -10.0f, 0.0f});
                        wheelBEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
                        wheelBEntity->component<Box2DCircleComponent>()->setFriction(5.0f);
                        wheelBEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setBodyA(wheelBEntity->component<Box2DBodyManager>());
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setBodyB(rectEntity->component<Box2DBodyManager>());
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setLocalAnchorB({-20, -10});
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setEnableMotor(true);
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                        wheelBEntity->component<Box2DRevoluteJointComponent>()->setMaxMotorTorque(1000.0f);

                        // Counter
                        auto counterEntity = sceneEntity->createEntity();
                        counterEntity->component<TextComponent>()->setText("0");
                        counterEntity->component<TransformComponent>()->setPos({-140.0f, 140.0f, 0.0f});
                        auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                        counterEntity->component<TextComponent>()->setFontRes(fontRes);

                        // Box2D ground
                        {
                            std::vector<glm::vec2> vertices(50);
                            float width = 320.0f;
                            float height = 300.0f;
                            float xStep = width / vertices.size();
                            for (int i = 0; i < vertices.size(); i++) {
                                vertices[i] = {i * xStep, std::sin(i * 0.2) * 10};
                            }
                            std::vector<glm::vec3> meshVertices(vertices.size() * 6);
                            for (int i = 0; i < vertices.size() - 1; i++) {
                                meshVertices[i * 6 + 0] = glm::vec3(vertices[i].x, vertices[i].y, 0.0f);
                                meshVertices[i * 6 + 1] = glm::vec3(vertices[i].x, -height * 0.5f, 0.0f);
                                meshVertices[i * 6 + 2] = glm::vec3(vertices[i + 1].x, vertices[i + 1].y, 0.0f);
                                meshVertices[i * 6 + 3] = glm::vec3(vertices[i + 1].x, vertices[i + 1].y, 0.0f);
                                meshVertices[i * 6 + 4] = glm::vec3(vertices[i].x, -height * 0.5f, 0.0f);
                                meshVertices[i * 6 + 5] = glm::vec3(vertices[i + 1].x, -height * 0.5f, 0.0f);
                            }
                            auto groundEntity = sceneEntity->createEntity();
                            groundEntity->component<MeshComponent>()->setVertices(meshVertices);
                            groundEntity->component<Box2DBodyManager>()->setType(b2_staticBody);
                            groundEntity->component<TransformComponent>()->setPos({-150.0f, -100.0f, 0.0f});
                            groundEntity->component<Box2DChainComponent>()->setVertices(vertices);
                        }

                        rootEntity->events()->subscribe([sceneEntity, counterEntity](MouseInputManager::MouseDownEvent e) {
                            float x = (e.pos.x - 300) * 0.5f;
                            float y = (300 - e.pos.y) * 0.5f;
                            LOG("Pos %f, %f", x, y);
                            auto circleEntity = sceneEntity->createEntity();
                            circleEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
                            circleEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
                            circleEntity->component<Box2DBodyManager>()->setType(b2_dynamicBody);
                            circleEntity->component<TransformComponent>()->setPos({x, y, 0.0f});
                            circleEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
                            circleEntity->component<Box2DCircleComponent>()->setFriction(0.3f);
                            circleEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
                            circleEntity->events()->subscribe([circleEntity, sceneEntity](Box2DWorldManager::ContactStartEvent) {
                                sceneEntity->removeEntity(circleEntity);
                            });
                            int count = std::stoi(counterEntity->component<TextComponent>()->text());
                            count++;
                            counterEntity->component<TextComponent>()->setText(std::to_string(count));
                        });

                        rootEntity->events()->subscribe([wheelAEntity, wheelBEntity](KeyboardInputManager::KeyDownEvent e) {
                            if (e.keyCode == KeyCode::LEFT) {
                                wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(-10.0f);
                                wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(-10.0f);
                            }
                            if (e.keyCode == KeyCode::RIGHT) {
                                wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(10.0f);
                                wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(10.0f);
                            }
                        });

                        rootEntity->events()->subscribe([wheelAEntity, wheelBEntity](KeyboardInputManager::KeyUpEvent e) {
                            if (e.keyCode == KeyCode::LEFT) {
                                wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                                wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                            }
                            if (e.keyCode == KeyCode::RIGHT) {
                                wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                                wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
                            }
                        });

                    });
    return application.runThread(currentThread);
}
