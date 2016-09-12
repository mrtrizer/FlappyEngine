#pragma once

#include <functional>
#include <glm/gtc/random.hpp>

#include <core/scene.h>
#include <core/transform.h>
#include <core/entitymanager.h>
#include <core/inputmanager.h>
#include <ui/button.h>

#include "ballctrl.h"
#include "basketctrl.h"
#include "gamectrl.h"

namespace game {

class MyScene : public flappy::Scene {
public:
    void init() override;

private:
    void createBasket(std::string spritePath, glm::vec2 pos);
};

} // game
