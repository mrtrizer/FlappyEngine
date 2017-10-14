#include "Utils.h"

namespace flappy {

std::vector<b2Vec2> glmVec2ToB2Vec2(std::vector<glm::vec2> vertices, float sizeFactor) {
    std::vector<b2Vec2> b2Vertices(vertices.size());
    std::transform(vertices.begin(), vertices.end(), b2Vertices.begin(), [sizeFactor](glm::vec2 vec) {
        return b2Vec2(vec.x * sizeFactor, vec.y * sizeFactor);
    });
    return b2Vertices;
}

}
