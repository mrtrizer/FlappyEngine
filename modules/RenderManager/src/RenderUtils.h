#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace flappy {

std::vector<glm::vec3> genCircleVertices(float r, int vertexN);

std::vector<glm::vec2> glmVec3ToVec2(std::vector<glm::vec3> vec3);

} // flappy
