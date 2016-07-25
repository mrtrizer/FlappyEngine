#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

class Texture {
public:
    struct UV {
        float u;
        float v;
    };

    Texture():uvs({{0,1},{0,0},{1,1},{1,0}}) {}
    virtual ~Texture() {}

    std::vector<UV> getUVs() const { return uvs; }
    float getRelWidth() const {return relWidth;}
    float getRelHeight() const {return relHeight;}

// TODO: Make private
protected:
    /// Saved UVs after auto resize
    std::vector<UV> uvs;
    /// Relative width (0.0 to 1.0)
    float relWidth = 1.0;
    /// Relative height (0.0 to 1.0)
    float relHeight = 1.0;
};

#endif // TEXTURE_H
