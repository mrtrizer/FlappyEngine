#pragma once

#include "rectshape.h"

namespace flappy {

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    Sprite(std::string path, float width, float height):
        RectShape(width,height),
        m_path(path)
    {}
    virtual ~Sprite(){}
    inline std::string path() const { return m_path; }

protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    std::string m_path;
};

} // flappy
