#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <Box2D.h>

namespace flappy {

std::vector<b2Vec2> glmVec2ToB2Vec2(std::vector<glm::vec2> vertices, float sizeFactor);

}
