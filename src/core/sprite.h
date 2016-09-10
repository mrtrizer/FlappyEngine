#pragma once

#include "rectshape.h"

#include <core/resourcemgr.h>

namespace flappy {

class Quad;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    void setPath(const std::string& path);
    std::string path() const { return m_path; }

    const std::shared_ptr<ResourceHandler<Quad>> quad() const { return m_quad; }

protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;

private:
    std::shared_ptr<ResourceHandler<Quad>> m_quad;
    std::string m_path;
};

} // flappy
