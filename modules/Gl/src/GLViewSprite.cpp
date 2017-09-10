#include "GLViewSprite.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLShaderProgram.h>
#include <GLTextureRes.h>
#include <ResManager.h>
#include <SpriteRes.h>
#include <SpriteComponent.h>

namespace flappy {

using namespace glm;
using namespace std;

static const char spriteVShader[] =
    "attribute vec2 aPosition;\n"
    "attribute vec2 aTexCoord;\n"
    "uniform mat4 uMVMatrix;\n"
    "uniform mat4 uPMatrix;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "   vTexCoord = aTexCoord;"
    "   gl_Position = uPMatrix * uMVMatrix * vec4(aPosition,0,1);\n"
    "}\n";

static const char spriteFShader[] =
#if defined(GL_ES) || EMSCRIPTEN || TARGET_OS_IPHONE == 1
    "precision mediump float;\n"
#endif
    "uniform sampler2D uTex;\n"
    "uniform vec4 uColor;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
#if TARGET_OS_IPHONE == 1
    "   gl_FragColor = texture2D(uTex,vTexCoord).rgba * uColor;\n"
#else
    "   gl_FragColor = texture2D(uTex,vTexCoord).bgra * uColor;\n"
#endif
    "}\n";

GLViewSprite::GLViewSprite(SafePtr<SpriteComponent> spriteComponent):
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
        m_shaderProgramRes = manager<ResManager<GLShaderProgram>>()->getRes("default_shape");
    });
}

void GLViewSprite::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
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

void GLViewSprite::updateFrame() {
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
