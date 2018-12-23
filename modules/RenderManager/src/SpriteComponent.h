#pragma once

#include <Color.h>
#include <SpriteRes.h>
#include <RenderElementFactory.h>
#include <Handle.hpp>

namespace flappy {

class Render;
    
class SpriteComponent {
public:
    SpriteComponent(Handle<Hierarchy> hierarchy);
    void setEntity(Handle<Entity> entity);
    ~SpriteComponent();

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setSpriteRes(std::shared_ptr<SpriteRes> quadRes) { m_quadRes = quadRes; }
    std::shared_ptr<SpriteRes> quadRes();

private:
    Color m_colorRGBA;
    std::shared_ptr<SpriteRes> m_quadRes;
    AnyHandle m_renderElement;
    Handle<Entity> m_entity;
    Handle<RenderElementFactory> m_renderElementFactory;
};

} // flappy
