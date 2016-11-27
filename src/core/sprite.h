#pragma once

#include "presenter.h"

namespace flappy {

class QuadRes;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public Presenter {
public:
    void setQuad(std::shared_ptr<QuadRes> quad) { m_quad = quad; }
    std::shared_ptr<QuadRes> quad() const {return m_quad; }

    unsigned id() override {return ClassId<Presenter,Sprite>::id(); }

private:
    std::shared_ptr<QuadRes> m_quad;
};

} // flappy
