#pragma once

#include <vector>

namespace flappy {

class Texture {
public:
    struct UV {
        float u;
        float v;
    };

    Texture():m_uvs({{0,1},{0,0},{1,1},{1,0}}) {}
    virtual ~Texture() = default;

    std::vector<UV> uvs() const { return m_uvs; }
    float relWidth() const {return m_relWidth;}
    float relHeight() const {return m_relHeight;}

// TODO: Make private
protected:
    /// Saved UVs after auto resize
    std::vector<UV> m_uvs;
    /// Relative width (0.0 to 1.0)
    float m_relWidth = 1.0;
    /// Relative height (0.0 to 1.0)
    float m_relHeight = 1.0;
};

} // flappy
