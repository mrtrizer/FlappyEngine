#pragma once

#include "PresenterComponent.h"

namespace flappy {

class QuadRes;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class SpriteComponent : public PresenterComponent {
public:
    void setQuad(std::shared_ptr<QuadRes> quad) { m_quad = quad; }
    std::shared_ptr<QuadRes> quad() const {return m_quad; }

    unsigned id() override {return ClassId<PresenterComponent,SpriteComponent>::id(); }

private:
    std::shared_ptr<QuadRes> m_quad;
};

} // flappy
