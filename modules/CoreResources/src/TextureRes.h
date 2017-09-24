#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <Res.h>

namespace flappy {

class TextureRes: public Res<TextureRes> {
public:
    struct UV {
        float u;
        float v;
    };

    TextureRes(glm::vec2 size):m_uvs({{0,1},{0,0},{1,1},{1,0}}), m_size(size) {}

    std::vector<UV> uvs() const { return m_uvs; }
    float relWidth() const {return m_relWidth;}
    float relHeight() const {return m_relHeight;}
    glm::vec2 size() const {return m_size;}

    // TODO: Make private
protected:
    /// Saved UVs after auto resize
    std::vector<UV> m_uvs;
    /// Relative width (0.0 to 1.0)
    float m_relWidth = 1.0;
    /// Relative height (0.0 to 1.0)
    float m_relHeight = 1.0;
    glm::vec2 m_size = {0.0f, 0.0f};
};

} // flappy
