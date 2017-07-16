#pragma once

#include <Component.h>
#include <Color.h>
#include <QuadRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>

namespace flappy {

class SpriteComponent: public Component<SpriteComponent> {
public:
    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color getColorRGBA() { return m_colorRGBA; }

    void setQuadRes(std::shared_ptr<QuadRes> quadRes) { m_quadRes = quadRes; }
    std::shared_ptr<QuadRes> getQuadRes() { return m_quadRes; }

    void init() override {
        m_renderElement = manager<RenderElementFactory>()->createSpriteRender(selfPointer<SpriteComponent>());
        entity()->addComponent(m_renderElement);
    }

    void deinit() override {
        entity()->removeComponent(m_renderElement);
        m_renderElement.reset();
    }

private:
    Color m_colorRGBA;
    std::shared_ptr<QuadRes> m_quadRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy