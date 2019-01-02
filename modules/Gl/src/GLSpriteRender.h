#pragma once

#include <Color.h>
#include <Handle.hpp>

#include <GLRender.h>
#include <GLAttribArray.h>

namespace flappy {

    class Entity;
    
template <typename T>
class ResHandler;

class SpriteRes;

class SpriteComponent;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLSpriteRender : public GLRender
{
public:
    GLSpriteRender(Handle<Entity> entity);
    void setEntity(Handle<Entity> entity);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_rect;
    std::vector<GLTools::Vertex> m_vertexList;
    std::shared_ptr<SpriteRes> m_quadRes;
    Handle<ResManager<ShaderRes>> m_shaderResManager;
    Handle<SpriteComponent> m_spriteComponent;

    void updateFrame();
};

} // flappy
