#include "glviewsprite.h"

#include <glm/gtc/type_ptr.hpp>
#include <core/presenter.h>
#include <core/sprite.h>
#include <gl/glshaderprogram.h>
#include <gl/gltexture.h>
#include <core/resourcemgr.h>

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
    "   gl_FragColor = texture2D(uTex,vTexCoord).rgba;\n"
#else
    "   gl_FragColor = texture2D(uTex,vTexCoord).bgra;\n"
#endif
    "}\n";

GLViewSprite::GLViewSprite(const Sprite & presenter) :
    GLView<GLViewSprite>(spriteVShader, spriteFShader),
    m_rect(GL_TRIANGLE_STRIP),
    m_vertexList{ {-0.5f, -0.5f},
                {-0.5f, 0.5f},
                {0.5f, -0.5f},
                {0.5f, 0.5f} }
{
    m_quad = presenter.quad();
    if (m_quad->ready())
        updateFrame();
}

void GLViewSprite::updateVBOs() {
    m_rect.reset(GL_TRIANGLE_STRIP);
    m_rect.addVBO<GLTools::Vertex>(m_vertexList.data(),
                                 static_cast<int>(m_vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));

    auto texture = m_quad->resource().texture();
    m_rect.addVBO<GLTexture::UV>(texture->resource().uvs().data(),
                                static_cast<int>(texture->resource().uvs().size()) * sizeof(GLTexture::UV),
                                GL_FLOAT,
                                getShader()->findAttr("aTexCoord"));

}

void GLViewSprite::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_quad != nullptr)
        if (m_quad->ready()) {
            if (m_quad->updated()) {
                updateFrame();
            }
            getShader()->render(m_rect, [this, mvMatrix, pMartrix](){
                auto texture = m_quad->resource().texture();
                glUniformMatrix4fv(getShader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
                glUniformMatrix4fv(getShader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
                glUniform4f(getShader()->findUniform("uColor"),0,0,0,1);
                dynamic_cast<GLTexture&>(texture->resource()).bind(getShader()->findUniform("uTex"), 0);
            });
        }
}

void GLViewSprite::updateFrame() {
    if (m_rect.size() != 4 * sizeof(GLTexture::UV))
        updateVBOs();
    auto texture = m_quad->resource().texture();
    auto rect = m_quad->resource().rect();
    float relWidth = texture->resource().relWidth();
    float relHeight = texture->resource().relHeight();
    float newRelWidth = relWidth * (rect.x2 - rect.x1);
    float newRelHeight = relHeight * (rect.y2 - rect.y1);
    float relX = rect.x1 * relWidth;
    float relY = rect.y1 * relHeight;
    vector<GLTexture::UV> uvs({
            {relX, relY + newRelHeight},
            {relX, relY},
            {relX + newRelWidth, relY + newRelHeight},
            {relX + newRelWidth, relY}});
    m_rect.vbo(1).writeData(uvs.data(),int(uvs.size()) * sizeof(GLTexture::UV));
}

void GLViewSprite::update(const Presenter & presenter){
    auto & presenterSprite = static_cast<const Sprite &>(presenter);

    m_quad = presenterSprite.quad();
    if (m_quad->ready())
        updateFrame();

}

} // flappy
