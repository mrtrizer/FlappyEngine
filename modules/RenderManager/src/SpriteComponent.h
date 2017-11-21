#pragma once

#include <Component.h>
#include <Color.h>
#include <SpriteRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>

namespace flappy {

class SpriteComponent: public Component<SpriteComponent> {
public:
    SpriteComponent();

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setSpriteRes(std::shared_ptr<SpriteRes> quadRes) { m_quadRes = quadRes; }
    std::shared_ptr<SpriteRes> quadRes();

private:
    Color m_colorRGBA;
    std::shared_ptr<SpriteRes> m_quadRes;
    std::shared_ptr<Render> m_renderElement;
};

} // flappy
