#include "RenderUtils.h"

#include <stdexcept>

namespace flappy {

/// Generates vertices for circle drawing in GL_TRIANGLE_FAN format
/// @param count Count of vertices ( >= 3).
std::vector<glm::vec3> genCircleVertices(float r, int vertexN)
{
    if (vertexN < 3)
        throw std::runtime_error("Too few vertices in circle (has to be >= 3).");
    std::vector<glm::vec3> vertexList(vertexN * 3);
    float step = M_PI * 2 / vertexN;
    for (int i = 0; i < vertexN; i++) {
        vertexList[i * 3 + 0] = {0.0f, 0.0f, 0.0f};
        vertexList[i * 3 + 1] = {cos(step * i) * r, sin(step * i) * r, 0.0f};
        int nextI = i + 1;
        vertexList[i * 3 + 2] = {cos(step * nextI) * r, sin(step * nextI) * r, 0.0f};
    }
    return vertexList;
}

std::vector<glm::vec2> glmVec3ToVec2(std::vector<glm::vec3> vec3Vertices) {
    std::vector<glm::vec2> vec2Vertices(vec3Vertices.size());
    std::transform(vec3Vertices.begin(), vec3Vertices.end(), vec2Vertices.begin(), [](const glm::vec3& item) {
        return glm::vec2(item.x, item.y);
    });
    return vec2Vertices;
}

} // flappy
