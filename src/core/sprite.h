#pragma once

#include "presenter.h"

#include "resmanager.h"

namespace flappy {

class Quad;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public Presenter {
public:
    void setPath(const std::string& path);
    std::string path() const { return m_path; }

    unsigned id() override {return ClassId<Presenter,Sprite>::id(); }

    const std::shared_ptr<ResHandler<Quad>> quad() const { return m_quad; }

private:
    std::shared_ptr<ResHandler<Quad>> m_quad;
    std::string m_path;
};

} // flappy
