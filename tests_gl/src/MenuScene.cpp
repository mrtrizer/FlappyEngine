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

    auto camera = CameraBuilder(shared_from_this())
            .size({960, 640})
            .build();
    manager<EntityManager>()->add(camera);
    setCamera(camera->get<CameraComponent>());

    auto layout = MenuLayoutBuilder(shared_from_this())
                    .build();
    manager<EntityManager>()->add(layout);

    layout->transform()->rotate(0.5f);

    events()->subscribe([this](MenuLayoutBuilder::StartButtonPressed){
        auto gameScene = std::make_shared<GameScene>();
        manager<SceneManager>()->setScene(gameScene);
    });
}

}
