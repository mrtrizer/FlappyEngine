#include "GLTextRender.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLShaderRes.h>
#include <GLTextureRes.h>
#include <ResManager.h>
#include <TextureRes.h>
#include <TextComponent.h>
#include <GlyphSheetRes.h>
#include <FontRes.h>
#include <MathUtils.h>

namespace flappy {

using namespace glm;
using namespace std;

GLTextRender::GLTextRender(Handle<Hierarchy> hierarchy)
    : GLRender(hierarchy)
    , m_rect(GL_TRIANGLE_STRIP)
    , m_shaderResManager(hierarchy->manager<ResManager<ShaderRes>>())
{
    setShader(m_shaderResManager->getRes("msdf_shader", ExecType::ASYNC));
}
    
void GLTextRender::setEntity(Handle<Entity> entity) {
    m_textComponent = entity->component<TextComponent>();
    m_fontRes = entity->component<TextComponent>()->fontRes();
    m_subscription = m_textComponent->eventBus().subscribe([this](TextComponent::TextChangedEvent) {
        m_textChanged = true;
    });
    GLRender::setEntity(entity);
}

void GLTextRender::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_textChanged) {
        updateFrame();
        m_textChanged = false;
    }
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

void GLTextRender::updateFrame() {
    auto glyphSheetRes = m_textComponent->fontRes()->glyphSheetRes();
    auto texture = m_textComponent->fontRes()->textureRes();
    auto str = m_textComponent->text();

    auto boxedText = TextComponent::genBoxedText(str, *glyphSheetRes, m_textComponent->maxWidth(), m_textComponent->size());

    vector<GLTextureRes::UV> uvs;
    std::vector<GLTools::Vertex> vertices;

    for (auto boxedLine : boxedText.boxedLines) {
        int xOffset = TextComponent::calcLineOffset(m_textComponent->align(), boxedText, boxedLine);
        for (auto lexem : boxedLine.boxedLexems) {
            for (auto box : lexem.boxes) {
                auto rectInAtlas = MathUtils::Rect(box.glyph.x / texture->size().x,
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
                vertices.push_back({box.rect.x1 + xOffset, -box.rect.y2 - boxedLine.yOffset});
                vertices.push_back({box.rect.x1 + xOffset, -box.rect.y1 - boxedLine.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, -box.rect.y2 - boxedLine.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, -box.rect.y2 - boxedLine.yOffset});
                vertices.push_back({box.rect.x1 + xOffset, -box.rect.y1 - boxedLine.yOffset});
                vertices.push_back({box.rect.x2 + xOffset, -box.rect.y1 - boxedLine.yOffset});
            }
            xOffset += lexem.width;
        }
    }

    m_rect.reset(GL_TRIANGLES);
    m_rect.addVBO<GLTextureRes::UV>(uvs, shader()->findAttr("aTexCoord"));
    m_rect.addVBO<GLTools::Vertex>(vertices, shader()->findAttr("aPosition"));
}

} // flappy
