#pragma once

#include <functional>
#include <glm/gtc/random.hpp>

#include <Scene.h>
#include <TransformComponent.h>
#include <EntityManager.h>
#include <InputManager.h>
#include <ButtonComponent.h>

#include "BallCtrl.h"
#include "BasketCtrl.h"
#include "GameCtrl.h"

namespace game {

class GameScene : public flappy::Scene {
public:
    void init() override;

private:
    void createBasket(std::string spritePath, glm::vec2 pos);
};

} // game
