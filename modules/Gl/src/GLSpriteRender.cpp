#include "GLSpriteRender.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLShaderRes.h>
#include <GLTextureRes.h>
#include <ResManager.h>
#include <SpriteRes.h>
#include <SpriteComponent.h>

namespace flappy {

using namespace glm;
using namespace std;

GLSpriteRender::GLSpriteRender(SafePtr<SpriteComponent> spriteComponent):
    m_rect(GL_TRIANGLE_STRIP),
    m_spriteComponent(spriteComponent),
    m_vertexList{ {-0.5f, -0.5f},
                {-0.5f, 0.5f},
                {0.5f, -0.5f},
                {0.5f, 0.5f} },
    m_quadRes(spriteComponent->quadRes())
{
    addDependency(SpriteComponent::id());

    subscribe([this](InitEvent) {
        setShader(manager<ResManager<GLShaderRes>>()->getRes("texture_shader", ExecType::ASYNC));
    });
}

void GLSpriteRender::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_quadRes != m_spriteComponent->quadRes()) {
        updateFrame();
        m_quadRes = m_spriteComponent->quadRes();
    }
    if (m_quadRes != nullptr) {
        shader()->render(m_rect, [this, mvMatrix, pMartrix](){
            auto texture = m_quadRes->texture();
            glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
            glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
            glUniform4fv(shader()->findUniform("uColor"), 1, reinterpret_cast<GLfloat *>(&m_spriteComponent->colorRGBA()));
            auto glTexture = static_pointer_cast<GLTextureRes>(texture);
            glTexture->bind(shader()->findUniform("uTex"), 0);
        });
    }
}

void GLSpriteRender::updateFrame() {
    auto texture = m_spriteComponent->quadRes()->texture();

    auto spriteInfo = m_spriteComponent->quadRes()->spriteInfo();

    auto rectInAtlas = spriteInfo.rectInAtlas;


    // Relative width and height of texture are useful for non square atlases.
    // We need to multiply relative sprite coords and relative atlas size.
    float textureRelativeWidth = texture->relWidth();
    float spriteRelativeWidth = textureRelativeWidth * rectInAtlas.size().x;
    float spriteRelativeX = rectInAtlas.x1 * textureRelativeWidth;

    float textureRelativeHeight = texture->relHeight();
    float spriteRelativeHeight = textureRelativeHeight * rectInAtlas.size().y;
    float spriteRelativeY = rectInAtlas.y1 * textureRelativeHeight;

    vector<GLTextureRes::UV> uvs({
            {spriteRelativeX, spriteRelativeY + spriteRelativeHeight},
            {spriteRelativeX, spriteRelativeY},
            {spriteRelativeX + spriteRelativeWidth, spriteRelativeY + spriteRelativeHeight},
            {spriteRelativeX + spriteRelativeWidth, spriteRelativeY}});

    m_rect.reset(GL_TRIANGLE_STRIP);

    m_rect.addVBO<GLTextureRes::UV>(uvs, shader()->findAttr("aTexCoord"));

    auto spriteSize = spriteInfo.size;

    m_vertexList = {
                {-0.5f * spriteSize.x, -0.5f * spriteSize.y},
                {-0.5f * spriteSize.x, 0.5f * spriteSize.y},
                {0.5f * spriteSize.x, -0.5f * spriteSize.y},
                {0.5f * spriteSize.x, 0.5f * spriteSize.y} };

    m_rect.addVBO<GLTools::Vertex>(m_vertexList, shader()->findAttr("aPosition"));
}

} // flappy
