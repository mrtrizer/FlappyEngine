#pragma once

#include "rectshape.h"

#include <core/resourcemanager.h>

namespace flappy {

class Quad;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public Presenter {
public:
    void setPath(const std::string& path);
    std::string path() const { return m_path; }

    int id() override {return ClassId<Presenter,Sprite>::id(); }

    const std::shared_ptr<ResourceHandler<Quad>> quad() const { return m_quad; }

private:
    std::shared_ptr<ResourceHandler<Quad>> m_quad;
    std::string m_path;
};

} // flappy
