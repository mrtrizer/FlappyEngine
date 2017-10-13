#include "GLViewText.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLShaderRes.h>
#include <GLTextureRes.h>
#include <ResManager.h>
#include <TextureRes.h>
#include <TextComponent.h>
#include <GlyphSheetRes.h>
#include <FontRes.h>

namespace flappy {

using namespace glm;
using namespace std;

GLViewText::GLViewText(SafePtr<TextComponent> textComponent):
    m_rect(GL_TRIANGLE_STRIP),
    m_textComponent(textComponent),
    m_fontRes(textComponent->fontRes())
{
    addDependency(TextComponent::id());

    subscribe([this](InitEvent) {
        setShader(manager<ResManager<GLShaderRes>>()->getRes("mcdf_shader", ExecType::ASYNC));
    });
}


void GLViewText::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_fontRes != m_textComponent->fontRes()) {
        updateFrame();
        m_fontRes = m_textComponent->fontRes();
    }
    if (m_fontRes != nullptr) {
        shader()->render(m_rect, [this, mvMatrix, pMartrix](){
            auto texture = m_fontRes->textureRes();
            glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
            glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
            glUniform4fv(shader()->findUniform("uColor"), 1, reinterpret_cast<GLfloat *>(&m_textComponent->colorRGBA()));
            auto glTexture = static_pointer_cast<GLTextureRes>(texture);
            glTexture->bind(shader()->findUniform("uTex"), 0);
        });
    }
}

void GLViewText::updateFrame() {
    auto glyphSheetRes = m_textComponent->fontRes()->glyphSheetRes();
    auto texture = m_textComponent->fontRes()->textureRes();
    auto str = m_textComponent->text();
    auto lineDistance = glyphSheetRes->common().lineHeight;

    auto boxedText = TextComponent::genBoxedText(str, *glyphSheetRes, m_textComponent->maxWidth());

    vector<GLTextureRes::UV> uvs;
    std::vector<GLTools::Vertex> vertices;

    for (auto line : boxedText.boxedLines) {
        int xOffset = 0;
        for (auto lexem : line.boxedLexems) {
            for (auto box : lexem.boxes) {
                auto rectInAtlas = Tools::Rect(box.glyph.x / texture->size().x,
                                               box.glyph.y / texture->size().y,
                                               (box.glyph.x + box.glyph.width) / texture->size().x,
                                               (box.glyph.y + box.glyph.height) / texture->size().y);

                float uvWidth = texture->relWidth() * rectInAtlas.size().x;
                float uvX = rectInAtlas.x1 * texture->relWidth();

                float uvHeight = texture->relHeight() * rectInAtlas.size().y;
                float uvY = rectInAtlas.y1 * texture->relWidth();

                uvs.push_back({uvX, uvY + uvHeight});
                uvs.push_back({uvX, uvY});
                uvs.push_back({uvX + uvWidth, uvY + uvHeight});
                uvs.push_back({uvX + uvWidth, uvY + uvHeight});
                uvs.push_back({uvX, uvY});
                uvs.push_back({uvX + uvWidth, uvY});

                // Notice that Y coord is inverted here because of different coord system
                // of glyph and GL.
                vertices.push_back({box.rect.x1 + xOffset, box.rect.y1 - line.yOffset});
                vertices.push_back({box.rect.x1 + xOffset, box.rect.y2 - line.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, box.rect.y1 - line.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, box.rect.y1 - line.yOffset});
                vertices.push_back({box.rect.x1 + xOffset, box.rect.y2 - line.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, box.rect.y2 - line.yOffset});
            }
            xOffset += lexem.width;
        }
    }

    m_rect.reset(GL_TRIANGLES);
    m_rect.addVBO<GLTextureRes::UV>(uvs, shader()->findAttr("aTexCoord"));
    m_rect.addVBO<GLTools::Vertex>(vertices, shader()->findAttr("aPosition"));
}

} // flappy
