#include <memory>

#include <BasicLoopManager.hpp>
#include <Sdl2Manager.h>
#include <SceneManager.h>
#include <GLRenderManager.h>
#include <GLRenderElementFactory.h>
#include <TransformComponent.h>
#include <ScreenManager.h>
#include <MeshComponent.h>
#include <CoreUtils.hpp>
#include <ResManager.h>
#include <StdFileMonitorManager.h>
#include <ResRepositoryManager.h>
#include <StdFileLoadManager.h>
#include <DefaultResFactory.h>
#include <GLShaderResFactory.h>
#include <TextResFactory.h>
#include <GLTextureResFactory.h>
#include <Box2DWorldManager.h>
#include <Box2DBodyComponent.h>
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
#include <DebugServices.h>
#include <Hierarchy.hpp>
#include <Heap.hpp>
#include <Entity.hpp>
#include <BasicLoop.hpp>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    // Sdl2 and render
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    
    hierarchy->initManager<BasicLoopManager>();
    hierarchy->initManager<UpdateManager>();
    auto sdl2Manager = hierarchy->initManager<IGLManager, Sdl2Manager>();
    auto mouseInputManager = hierarchy->initManager<MouseInputManager> ();
    hierarchy->initManager<Sdl2MouseInput> ();
    auto keyboardInputManager = hierarchy->initManager<KeyboardInputManager> ();
    hierarchy->initManager<Sdl2KeyboardInput> ();

    auto sceneManager = hierarchy->initManager<SceneManager>();
    auto screenManager = hierarchy->initManager<ScreenManager>();
    screenManager->resize({600, 600});

    hierarchy->initManager<RenderManager, GLRenderManager>();
    hierarchy->initManager<RenderElementFactory, GLRenderElementFactory>();

    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();

    auto resRepository = hierarchy->initManager<ResRepositoryManager>();
    resRepository->setRepositoryPath("./resources");

    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    hierarchy->initManager<ResManager<TextRes>> ();

    hierarchy->initManager<ResFactory<ShaderRes>, GLShaderResFactory> ();
    hierarchy->initManager<ResManager<ShaderRes>> ();

    hierarchy->initManager<ResFactory<JsonRes>, DefaultResFactory<JsonRes, JsonRes, TextRes>>();
    hierarchy->initManager<ResManager<JsonRes>>();

    hierarchy->initManager<ResFactory<IRgbaBitmapRes>, Sdl2RgbaBitmapResFactory> ();
    hierarchy->initManager<ResManager<IRgbaBitmapRes>> ();

    hierarchy->initManager<ResFactory<TextureRes>, GLTextureResFactory>();
    hierarchy->initManager<ResManager<TextureRes>> ();

    hierarchy->initManager<ResFactory<GlyphSheetRes>, GlyphSheetResFactory> ();
    hierarchy->initManager<ResManager<GlyphSheetRes>> ();

    hierarchy->initManager<ResFactory<FontRes>, FontResFactory> ();
    auto fontResManager = hierarchy->initManager<ResManager<FontRes>>();

    hierarchy->initManager<ResFactory<MaterialRes>, MaterialResFactory> ();
    hierarchy->initManager<ResManager<MaterialRes>> ();


    // Box2D world
    auto box2dWorldManager = hierarchy->initManager<Box2DWorldManager>();
    box2dWorldManager->setVelocityIterations(6);
    box2dWorldManager->setPositionIterations(2);
    box2dWorldManager->setGravity({0.0f, -10.0f});
    box2dWorldManager->setSizeFactor(0.1f);

    // Scene
    auto sceneEntity = hierarchy->rootEntity()->createEntity();
    sceneManager->setMainCamera(sceneEntity->component<CameraComponent>());
    sceneEntity->component<CameraComponent>()->setSize({300, 300});

    // Dynamic box
    auto rectEntity = sceneEntity->createEntity();
    rectEntity->component<MeshComponent>();
    rectEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
    rectEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
    rectEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 0.0f});
    rectEntity->component<Box2DBoxComponent>()->setDensity(10.0f);
    rectEntity->component<Box2DBoxComponent>()->setFriction(0.3f);
    rectEntity->component<Box2DBoxComponent>()->setSize({20.0f, 20.0f});

    // Dynamic circle
    auto wheelAEntity = sceneEntity->createEntity();
    wheelAEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
    wheelAEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
    wheelAEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
    wheelAEntity->component<TransformComponent>()->setPos({20.0f, -10.0f, 0.0f});
    wheelAEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
    wheelAEntity->component<Box2DCircleComponent>()->setFriction(5.0f);
    wheelAEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setBodyA(wheelAEntity->component<Box2DBodyComponent>());
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setBodyB(rectEntity->component<Box2DBodyComponent>());
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setLocalAnchorB({20, -10});
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setEnableMotor(true);
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
    wheelAEntity->component<Box2DRevoluteJointComponent>()->setMaxMotorTorque(1000.0f);

    // Dynamic circle
    auto wheelBEntity = sceneEntity->createEntity();
    wheelBEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
    wheelBEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
    wheelBEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
    wheelBEntity->component<TransformComponent>()->setPos({-20.0f, -10.0f, 0.0f});
    wheelBEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
    wheelBEntity->component<Box2DCircleComponent>()->setFriction(5.0f);
    wheelBEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
    wheelBEntity->component<Box2DRevoluteJointComponent>()->setBodyA(wheelBEntity->component<Box2DBodyComponent>());
    wheelBEntity->component<Box2DRevoluteJointComponent>()->setBodyB(rectEntity->component<Box2DBodyComponent>());
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
    auto groundEntity = sceneEntity->createEntity();
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
        
        groundEntity->component<MeshComponent>()->setVertices(meshVertices);
        groundEntity->component<Box2DBodyComponent>()->setType(b2_staticBody);
        groundEntity->component<TransformComponent>()->setPos({-150.0f, -100.0f, 0.0f});
        groundEntity->component<Box2DChainComponent>()->setVertices(vertices);
    }

    std::list<shared_ptr<ISubscription>> contactSubscriptions;
    auto downSub = mouseInputManager->eventBus().subscribe(
        [sceneEntity, counterEntity, groundEntity, &contactSubscriptions] (MouseInputManager::MouseDownEvent e) {
            float x = (e.pos.x - 300) * 0.5f;
            float y = (300 - e.pos.y) * 0.5f;
            LOG("Pos %f, %f", x, y);
            auto circleEntity = sceneEntity->createEntity();
            circleEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,10));
            circleEntity->component<TransformComponent>()->setScale({20.0f, 20.0f});
            circleEntity->component<Box2DBodyComponent>()->setType(b2_dynamicBody);
            circleEntity->component<TransformComponent>()->setPos({x, y, 0.0f});
            circleEntity->component<Box2DCircleComponent>()->setDensity(10.0f);
            circleEntity->component<Box2DCircleComponent>()->setFriction(0.3f);
            circleEntity->component<Box2DCircleComponent>()->setRadius(10.0f);
            auto shape = circleEntity->component<Box2DCircleComponent>();
            contactSubscriptions.emplace_back(shape->eventBus().subscribe(
                [circleEntity, groundEntity](Box2DFixtureComponent::ContactStartEvent e) {
                    if (groundEntity->component<Box2DChainComponent>().objectId() == e.otherFixtureId)
                        CoreUtils::removeEntityDelayed(circleEntity);
                }));
            int count = std::stoi(counterEntity->component<TextComponent>()->text());
            count++;
            counterEntity->component<TextComponent>()->setText(std::to_string(count));
        });

    auto keyDownSub = keyboardInputManager->eventBus().subscribe([wheelAEntity, wheelBEntity, rectEntity](KeyboardInputManager::KeyDownEvent e) {
        if (e.keyCode == KeyCode::LEFT) {
            wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(-10.0f);
            wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(-10.0f);
        }
        if (e.keyCode == KeyCode::RIGHT) {
            wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(10.0f);
            wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(10.0f);
        }
        if (e.keyCode == KeyCode::SPACE) {
            rectEntity->component<Box2DBodyComponent>()->applyForce({0.0f, 10000.0f}, {0.0f, 0.0f});
        }
    });

    auto keyUpSub = keyboardInputManager->eventBus().subscribe([wheelAEntity, wheelBEntity](KeyboardInputManager::KeyUpEvent e) {
        if (e.keyCode == KeyCode::LEFT) {
            wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
            wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
        }
        if (e.keyCode == KeyCode::RIGHT) {
            wheelAEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
            wheelBEntity->component<Box2DRevoluteJointComponent>()->setMotorSpeed(0.0f);
        }
    });

    return BasicLoop(30, hierarchy->manager<BasicLoopManager>()).run([&hierarchy](DeltaTime dt) {
        hierarchy->manager<UpdateManager>()->update(dt);
        hierarchy->manager<Sdl2Manager>()->update(dt);
    });
}
