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
    auto camera = manager<EntityManager>()->create();
    setCamera(camera->component<CameraComponent>()
              ->setSize({960, 640}));

    auto layout = manager<EntityManager>()->create();
    layout->component<MenuLayoutComponent>();
    layout->component<TransformComponent>()->rotate(0.5f);

    events()->subscribe([this](MenuLayoutComponent::StartButtonPressed){
        auto gameScene = std::make_shared<GameScene>();
        manager<SceneManager>()->setScene(gameScene);
    });
}

}
