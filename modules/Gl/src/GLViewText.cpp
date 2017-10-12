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
    m_spriteComponent(textComponent),
    m_fontRes(textComponent->fontRes())
{
    addDependency(TextComponent::id());

    subscribe([this](InitEvent) {
        setShader(manager<ResManager<GLShaderRes>>()->getRes("mcdf_shader", ExecType::ASYNC));
    });
}


void GLViewText::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    if (m_fontRes != m_spriteComponent->fontRes()) {
        updateFrame();
        m_fontRes = m_spriteComponent->fontRes();
    }
    if (m_fontRes != nullptr) {
        shader()->render(m_rect, [this, mvMatrix, pMartrix](){
            auto texture = m_fontRes->textureRes();
            glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
            glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
            glUniform4fv(shader()->findUniform("uColor"), 1, reinterpret_cast<GLfloat *>(&m_spriteComponent->colorRGBA()));
            auto glTexture = static_pointer_cast<GLTextureRes>(texture);
            glTexture->bind(shader()->findUniform("uTex"), 0);
        });
    }
}

void GLViewText::updateFrame() {
    auto glyphSheetRes = m_spriteComponent->fontRes()->glyphSheetRes();
    auto texture = m_spriteComponent->fontRes()->textureRes();
    auto str = m_spriteComponent->text();
    int size = m_spriteComponent->size();

    vector<GLTextureRes::UV> uvs;
    std::vector<GLTools::Vertex> vertices;

    for (int i = 0; i < str.length(); i++) {
        char symbol = str[i];

        auto glyph = glyphSheetRes->glyph((int)symbol);

        auto rectInAtlas = Tools::Rect(glyph.x / texture->size().x,
                                       glyph.y / texture->size().y,
                                       (glyph.x + glyph.width) / texture->size().x,
                                       (glyph.y + glyph.height) / texture->size().y);

        float uvWidth = texture->relWidth() * rectInAtlas.size().x;
        float uvX = rectInAtlas.x1 * texture->relWidth();

        float uvHeight = texture->relHeight() * rectInAtlas.size().y;
        float uvY = rectInAtlas.y1 * texture->relWidth();

        uvs.push_back({uvX, uvY + uvHeight});
        uvs.push_back({uvX, uvY});
        uvs.push_back({uvX + uvWidth, uvY + uvHeight});
        uvs.push_back({uvX + uvWidth, uvY});

        int offsetX = size * i;

        vertices.push_back({-0.5f * size + offsetX, -0.5f * size});
        vertices.push_back({-0.5f * size + offsetX, 0.5f * size});
        vertices.push_back({0.5f * size + offsetX, -0.5f * size});
        vertices.push_back({0.5f * size + offsetX, 0.5f * size});
    }

    m_rect.reset(GL_TRIANGLE_STRIP);
    m_rect.addVBO<GLTextureRes::UV>(uvs, shader()->findAttr("aTexCoord"));
    m_rect.addVBO<GLTools::Vertex>(vertices, shader()->findAttr("aPosition"));
}

} // flappy
