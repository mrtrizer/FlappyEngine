#pragma once

#include <Component.h>
#include <Color.h>
#include <SpriteRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>

namespace flappy {

class SpriteComponent: public Component<SpriteComponent> {
public:
    SpriteComponent()
    {
        addDependency(RenderElementFactory::id());

        subscribe([this](InitEvent) {
            m_renderElement = manager<RenderElementFactory>()->createSpriteRender(selfPointer());
            entity()->addComponent(m_renderElement);
        });

        subscribe([this](DeinitEvent) {
            entity()->removeComponent(m_renderElement);
            m_renderElement.reset();
        });
    }

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setSpriteRes(std::shared_ptr<SpriteRes> quadRes) { m_quadRes = quadRes; }
    std::shared_ptr<SpriteRes> quadRes() {
        if (m_quadRes != nullptr && m_quadRes->resUpdated()) {
            m_quadRes = std::static_pointer_cast<SpriteRes>(m_quadRes->lastRes());
        }
        return m_quadRes;
    }

private:
    Color m_colorRGBA;
    std::shared_ptr<SpriteRes> m_quadRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy
