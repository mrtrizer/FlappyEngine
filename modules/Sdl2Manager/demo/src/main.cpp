#include <memory>

#include <Sdl2Manager.h>
#include <Heap.hpp>
#include <Entity.hpp>
#include <Hierarchy.hpp>
#include <SceneManager.h>
#include <GLRenderManager.h>
#include <GLRenderElementFactory.h>
#include <BasicLoopManager.hpp>
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
#include <BasicLoop.hpp>

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
    // Sdl2 and render
    auto hierarchy = Heap::create<Hierarchy>();
    auto rootEntity = hierarchy->rootEntity();
    hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<ScreenManager>(600, 600);
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto repoManager = hierarchy->initManager<ResRepositoryManager>("./resources");
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

    // Scene
    sceneManager->setMainCamera(rootEntity->component<CameraComponent>());
    rootEntity->component<CameraComponent>()->setSize({600, 600});

    // Sprite
    {
        auto spriteEntity = rootEntity->createEntity();
        spriteEntity->component<TransformComponent>()->setAngle2DRad(M_PI / 4);
        spriteEntity->component<TransformComponent>()->setPos({100.0f, 1.0f, 1.0f});
        auto spriteRes = spriteResManager->getRes("demo_atlas:blue_button01", ExecType::ASYNC);
        spriteEntity->component<SpriteComponent>()->setSpriteRes(spriteRes);
    }

    // Single image sprite
    {
        auto spriteEntity = rootEntity->createEntity();
        spriteEntity->component<TransformComponent>()->setPos({0.0f, 0.0f, 1.0f});

        auto spriteRes = spriteResManager->getRes("__img_missing__", ExecType::ASYNC);
        spriteEntity->component<SpriteComponent>()->setSpriteRes(spriteRes);
    }

    // Shape
    auto circleEntity = rootEntity->createEntity();
    circleEntity->component<TransformComponent>()->setPos({-150.0f, 0.0f, 0.0f});
    circleEntity->component<TransformComponent>()->setAngle2DRad(M_PI * 0.1f);
    circleEntity->component<MeshComponent>()->setVertices(genCircleVertices(0.5f,30));
    auto testMaterial = materialManager->getRes("test_material", ExecType::ASYNC);
    circleEntity->component<MeshComponent>()->setMaterialRes(testMaterial);

    // Text
    auto textEntity = rootEntity->createEntity();
    textEntity->component<TransformComponent>()->setPos({-300.0f, 200.0f, 0.0f});
    textEntity->component<TextComponent>()->setText("AVTest ВАСЯ 123\n456 789");
    textEntity->component<TextComponent>()->setAlign(TextComponent::Align::CENTER);
    textEntity->component<TextComponent>()->setMaxWidth(170);
    textEntity->component<TextComponent>()->setSize(30);
    auto fontRes = fontResManager->getRes("irohamaru-mikami-Medium", ExecType::ASYNC);
    textEntity->component<TextComponent>()->setFontRes(fontRes);

    return BasicLoop(30, basicLoopManager).run([&hierarchy
                                                ,&circleEntity
                                                ,&sdl2Manager
                                                ](float dt) {
        hierarchy->manager<UpdateManager>()->update(dt);
        sdl2Manager->update(dt);
        circleEntity->component<TransformComponent>()->rotate2DRad(dt);
        float angle = circleEntity->component<TransformComponent>()->angle2DRad();
        circleEntity->component<MeshComponent>()->setVertices(genSinCircleVertices(50.f, 30, angle));
    });
}
