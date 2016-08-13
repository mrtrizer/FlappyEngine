#include <glm/gtc/type_ptr.hpp>
#include <core/presenter.h>
#include <core/sprite.h>

#include "glviewsprite.h"

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

GLViewSprite::GLViewSprite(const shared_ptr<ResourceHandler<Texture>> &glTexture, const Sprite & presenter) :
    GLView<GLViewSprite>(spriteVShader, spriteFShader),
    rect(GL_TRIANGLE_STRIP),
    texture(glTexture),
    vertexList{ {-presenter.width() / 2,-presenter.height() / 2},
                {-presenter.width() / 2,presenter.height() / 2},
                {presenter.width() / 2,-presenter.height() / 2},
                {presenter.width() / 2,presenter.height() / 2} },
    lastFrameCount(presenter.frameCnt()),
    lastFrameN(presenter.frameN())
{
    if (texture->ready())
        updateVBOs();
}

void GLViewSprite::updateVBOs() {
    rect.reset(GL_TRIANGLE_STRIP);
    rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));

    rect.addVBO<GLTexture::UV>(texture->resource().uvs().data(),
                                static_cast<int>(texture->resource().uvs().size()) * sizeof(GLTexture::UV),
                                GL_FLOAT,
                                getShader()->findAttr("aTexCoord"));

    updateFrame(lastFrameCount, lastFrameN);
}

void GLViewSprite::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (texture->ready()) {
        if (texture->updated()) {
            updateVBOs();
        }
        getShader()->render(rect, [this, mvMatrix, pMartrix](){
            glUniformMatrix4fv(getShader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
            glUniformMatrix4fv(getShader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
            glUniform4f(getShader()->findUniform("uColor"),0,0,0,1);
            dynamic_cast<GLTexture&>(texture->resource()).bind(getShader()->findUniform("uTex"), 0);
        });
    }
}

void GLViewSprite::updateFrame(int frameCnt, int frameN) {
    float relWidth = texture->resource().relWidth();
    float relHeight = texture->resource().relHeight();
    float newRelWidth = relWidth / frameCnt;
    float newRelHeight = relHeight;
    float relX = newRelWidth * frameN;
    float relY = 0;
    vector<GLTexture::UV> uvs({
            {relX,relY + newRelHeight},
            {relX,relY},
            {relX + newRelWidth,relY + newRelHeight},
            {relX + newRelWidth,relY}});
    rect.vbo(1).writeData(uvs.data(),int(uvs.size()) * sizeof(GLTexture::UV));
}

void GLViewSprite::update(const Presenter & presenter){
    auto & presenterSprite = static_cast<const Sprite &>(presenter);
    updateFrame(presenterSprite.frameN(), presenterSprite.frameCnt());
}

} // flappy
