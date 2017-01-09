#include "MenuScene.h"

#include <EntityManager.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <QuadRes.h>
#include <ResManager.h>
#include <SizeComponent.h>
#include <CameraBuilder.h>

#include "GameScene.h"
#include "MenuLayoutBuilder.h"

namespace game {

using namespace flappy;

void MenuScene::init() {
    auto camera = manager<EntityManager>()->add(
        CameraBuilder()
            .size({960, 640})
    );
    setCamera(camera->get<CameraComponent>());

    auto layout = manager<EntityManager>()->add(
        MenuLayoutBuilder()
    );

    layout->transform()->rotate(0.5f);

    events()->subscribe([this](MenuLayoutBuilder::StartButtonPressed){
        auto gameScene = std::make_shared<GameScene>();
        manager<SceneManager>()->setScene(gameScene);
    });
}

}
