#pragma once

#include <Component.h>
#include <Color.h>
#include <QuadRes.h>

namespace flappy {

class SpriteComponent: public Component {
public:
    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color getColorRGBA() { return m_colorRGBA; }

    void setQuadRes(std::shared_ptr<QuadRes> quadRes) { m_quadRes = quadRes; }
    std::shared_ptr<QuadRes> getQuadRes() { return m_quadRes; }

private:
    Color m_colorRGBA;
    std::shared_ptr<QuadRes> m_quadRes;
};

} // flappy
