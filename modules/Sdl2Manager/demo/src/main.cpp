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
#include <TextResFactory.h>
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

std::vector<glm::vec3> genSinCircleVertices(float r, int vertexN, float offset)
{
    if (vertexN < 3)
        throw std::runtime_error("Too few vertices in circle (has to be >= 3).");
    std::vector<glm::vec3> vertexList(vertexN * 3);
    const float factor = 1.0f;
    const float amplitude = 10.0f;
    float step = M_PI * 2 / vertexN;
    for (int i = 0; i < vertexN; i++) {
        vertexList[i * 3 + 0] = {0.0f, 0.0f, 0.0f};
        float rCur = r + sin(i * factor + offset) * amplitude + sin(i * factor * 0.5f) * amplitude;
        float rNext = r + sin((i + 1) * factor + offset) * amplitude + sin((i + 1) * factor * 0.5f) * amplitude;
        vertexList[i * 3 + 1] = {cos(step * i) * rCur, sin(step * i) * rCur, 0.0f};
        int nextI = i + 1;
        vertexList[i * 3 + 2] = {cos(step * nextI) * rNext, sin(step * nextI) * rNext, 0.0f};
    }
    return vertexList;
}

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
                        rootEntity->createComponent<TextResFactory>();
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
                        auto materialManager = rootEntity->createComponent<ResManager<MaterialRes>> ();
                        auto fontResManager = rootEntity->createComponent<ResManager<FontRes>>();

                        // Scene
                        auto sceneEntity = rootEntity->createEntity();
                        sceneEntity->component<SceneManager>()->setMainCamera(sceneEntity->component<CameraComponent>());
                        sceneEntity->component<CameraComponent>()->setSize({600, 600});
                        sceneEntity->component<GLRenderManager>();
                        sceneEntity->component<GLRenderElementFactory>();

                        // Sprite
                        {
                            auto spriteEntity = sceneEntity->createEntity();
                            spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
                            spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});
                            auto spriteResManager = rootEntity->component<ResManager<SpriteRes>>();
                            auto spriteRes = spriteResManager->getRes("demo_atlas:blue_button01", ExecType::ASYNC);
                            spriteEntity->component<SpriteComponent>()->setSpriteRes(spriteRes);
                        }

                        // Single image sprite
                        {
                            auto spriteEntity = sceneEntity->createEntity();
                            spriteEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 1.0f});
                            auto spriteResManager = rootEntity->component<ResManager<SpriteRes>>();
                            auto spriteRes = spriteResManager->getRes("__img_missing__", ExecType::ASYNC);
                            spriteEntity->component<SpriteComponent>()->setSpriteRes(spriteRes);
                        }

                        // Shape
                        auto circleEntity = sceneEntity->createEntity();
                        circleEntity->component<TransformComponent>()->setPos({-150.0f, 0.0f, 0.0f});
                        circleEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
                        circleEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,30));
                        auto testMaterial = materialManager->getRes("test_material", ExecType::ASYNC);
                        circleEntity->component<MeshComponent>()->setMaterialRes(testMaterial);

                        // Text
                        auto textEntity = sceneEntity->createEntity();
                        textEntity->component<TransformComponent>()->setPos({-300.0f, 200.0f, 0.0f});
                        textEntity->component<TextComponent>()->setText("AVTest ВАСЯ 123\n456 789");
                        textEntity->component<TextComponent>()->setAlign(TextComponent::Align::CENTER);
                        textEntity->component<TextComponent>()->setMaxWidth(170);
                        textEntity->component<TextComponent>()->setSize(30);
                        auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
                        textEntity->component<TextComponent>()->setFontRes(fontRes);

                        rootEntity->events()->subscribe([circleEntity](ComponentBase::UpdateEvent e) {
                            circleEntity->component<TransformComponent>()->rotate2DRad(e.dt);
                            float angle = circleEntity->component<TransformComponent>()->angle2DRad();
                            circleEntity->component<MeshComponent>()->setVertices(genSinCircleVertices(50.f, 30, angle));
                        });

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
