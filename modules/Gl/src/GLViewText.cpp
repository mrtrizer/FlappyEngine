#include "GLViewText.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLShaderRes.h>
#include <GLTextureRes.h>
#include <ResManager.h>
#include <TextureRes.h>
#include <TextComponent.h>
#include <GlyphSheetRes.h>

namespace flappy {

using namespace glm;
using namespace std;

GLViewText::GLViewText(SafePtr<TextComponent> spriteComponent):
    m_rect(GL_TRIANGLE_STRIP),
    m_spriteComponent(spriteComponent),
    m_vertexList{ {-0.5f, -0.5f},
                {-0.5f, 0.5f},
                {0.5f, -0.5f},
                {0.5f, 0.5f} },
    m_textureRes(spriteComponent->textureRes())
{
    addDependency(TextComponent::id());

    subscribe([this](InitEvent) {
        setShader(manager<ResManager<GLShaderRes>>()->getRes("mcdf_shader", ExecType::ASYNC));
    });
}


void GLViewText::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_textureRes != m_spriteComponent->textureRes()) {
        updateFrame();
        m_glyphSheetRes = m_spriteComponent->glyphSheetRes();
        m_textureRes = m_spriteComponent->textureRes();
    }
    if (m_textureRes != nullptr) {
        shader()->render(m_rect, [this, mvMatrix, pMartrix](){
            auto texture = m_textureRes;
            glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
            glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
            glUniform4fv(shader()->findUniform("uColor"), 1, reinterpret_cast<GLfloat *>(&m_spriteComponent->colorRGBA()));
            auto glTexture = static_pointer_cast<GLTextureRes>(texture);
            glTexture->bind(shader()->findUniform("uTex"), 0);
        });
    }
}

void GLViewText::updateFrame() {
    auto glyphSheetRes = m_spriteComponent->glyphSheetRes();
    auto texture = m_spriteComponent->textureRes();
    auto str = m_spriteComponent->text();

    vector<GLTextureRes::UV> uvs;

    m_vertexList.clear();

    for (int i = 0; i < str.length(); i++) {
        char symbol = str[i];

        auto glyph = glyphSheetRes->glyph((int)symbol);

        auto rectInAtlas = Tools::Rect(glyph.x / texture->size().x,
                                       glyph.y / texture->size().y,
                                       (glyph.x + glyph.width) / texture->size().x,
                                       (glyph.y + glyph.height) / texture->size().y);

        // Relative width and height of texture are useful for non square atlases.
        // We need to multiply relative sprite coords and relative atlas size.
        float textureRelativeWidth = texture->relWidth();
        float spriteRelativeWidth = textureRelativeWidth * rectInAtlas.size().x;
        float spriteRelativeX = rectInAtlas.x1 * textureRelativeWidth;

        float textureRelativeHeight = texture->relHeight();
        float spriteRelativeHeight = textureRelativeHeight * rectInAtlas.size().y;
        float spriteRelativeY = rectInAtlas.y1 * textureRelativeHeight;

        uvs.push_back({spriteRelativeX, spriteRelativeY + spriteRelativeHeight});
        uvs.push_back({spriteRelativeX, spriteRelativeY});
        uvs.push_back({spriteRelativeX + spriteRelativeWidth, spriteRelativeY + spriteRelativeHeight});
        uvs.push_back({spriteRelativeX + spriteRelativeWidth, spriteRelativeY});

        m_rect.reset(GL_TRIANGLE_STRIP);

        auto spriteSize = glm::vec2(20,20);
        int offset = spriteSize.x * i;

        m_vertexList.push_back({-0.5f * spriteSize.x + offset, -0.5f * spriteSize.y});
        m_vertexList.push_back({-0.5f * spriteSize.x + offset, 0.5f * spriteSize.y});
        m_vertexList.push_back({0.5f * spriteSize.x + offset, -0.5f * spriteSize.y});
        m_vertexList.push_back({0.5f * spriteSize.x + offset, 0.5f * spriteSize.y});

    }

    m_rect.addVBO<GLTextureRes::UV>(uvs, shader()->findAttr("aTexCoord"));

    m_rect.addVBO<GLTools::Vertex>(m_vertexList, shader()->findAttr("aPosition"));
}

} // flappy
