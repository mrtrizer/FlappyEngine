#pragma once

#include "rectshape.h"

#include <core/resourcemgr.h>

namespace flappy {

class Quad;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    Sprite(std::string path, float width, float height):
        RectShape(width,height),
        m_path(path)
    {}

    virtual ~Sprite(){}
    void setPath(const std::string& path)
    {
        auto quadPath = path;
        bool isImgPath = true;
        for (char& c: quadPath)
            if (c == ':')
                isImgPath = false;
        if (isImgPath)
            quadPath = std::string(":") + quadPath;
        m_quad = MGR<ResourceMgr>()->get<Quad>(quadPath);
        updateView();
    }

    const std::shared_ptr<ResourceHandler<Quad>> quad() const { return m_quad; }

    void init() override {
        setPath(m_path);
    }


protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    std::shared_ptr<ResourceHandler<Quad>> m_quad;
    std::string m_path;
};

} // flappy
