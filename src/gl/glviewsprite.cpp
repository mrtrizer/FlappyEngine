#include <glm/gtc/type_ptr.hpp>
#include <core/presenter.h>
#include <core/sprite.h>

#include "glviewsprite.h"

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

GLViewSprite::GLViewSprite(const std::shared_ptr<GLTexture> &glTexture, const Sprite & presenter) :
    GLView<GLViewSprite>(spriteVShader, spriteFShader),
    rect(GL_TRIANGLE_STRIP),
    texture(glTexture),
    vertexList({
                {-presenter.getWidth() / 2,-presenter.getHeight() / 2},
                {-presenter.getWidth() / 2,presenter.getHeight() / 2},
                {presenter.getWidth() / 2,-presenter.getHeight() / 2},
                {presenter.getWidth() / 2,presenter.getHeight() / 2} }){

    rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));

    rect.addVBO<GLTexture::UV>(texture->getUVs().data(),
                                static_cast<int>(texture->getUVs().size()) * sizeof(GLTexture::UV),
                                GL_FLOAT,
                                getShader()->findAttr("aTexCoord"));

    update(presenter);
}

void GLViewSprite::draw(const glm::mat4 &pMartrix, const glm::mat4 &mvMatrix) {
    getShader()->render(rect, [this, mvMatrix, pMartrix](){
        glUniformMatrix4fv(getShader()->findUniform("uMVMatrix"),1,false,glm::value_ptr(mvMatrix));
        glUniformMatrix4fv(getShader()->findUniform("uPMatrix"),1,false,glm::value_ptr(pMartrix));
        glUniform4f(getShader()->findUniform("uColor"),0,0,0,1);
        texture->bind(getShader()->findUniform("uTex"), 0);
    });
}

void GLViewSprite::update(const Presenter & presenter){
    auto & presenterSprite = dynamic_cast<const Sprite &>(presenter);
    int frameCnt = presenterSprite.getFrameCnt();
    int frameN = presenterSprite.getFrameN();
    float relWidth = texture->getRelWidth();
    float relHeight = texture->getRelHeight();
    float newRelWidth = relWidth / frameCnt;
    float newRelHeight = relHeight;
    float relX = newRelWidth * frameN;
    float relY = 0;
    std::vector<GLTexture::UV> uvs({
            {relX,relY + newRelHeight},
            {relX,relY},
            {relX + newRelWidth,relY + newRelHeight},
            {relX + newRelWidth,relY}});
    rect.getVBO(1).writeData(uvs.data(),static_cast<int>(uvs.size()) * sizeof(GLTexture::UV));
}
