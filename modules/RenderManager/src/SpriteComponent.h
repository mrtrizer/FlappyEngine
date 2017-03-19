#pragma once

#include <QuadRes.h>

#include "PresenterComponent.h"

namespace flappy {

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class SpriteComponent : public PresenterComponent {
public:
    void setQuad(std::shared_ptr<QuadRes> quad) { m_quad = quad; }
    void setSpriteResByPath(std::string path) {
        setQuad(manager<ResManager>()->getRes<QuadRes>(path));
    }
    std::shared_ptr<QuadRes> quad() const {return m_quad; }

    unsigned id() override {return ClassId<PresenterComponent,SpriteComponent>::id(); }

private:
    std::shared_ptr<QuadRes> m_quad;
};

} // flappy
