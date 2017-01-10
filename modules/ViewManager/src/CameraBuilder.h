#pragma once

#include <Builder.h>

namespace flappy {

class CameraBuilder : public Builder
{
public:
    using Builder::Builder;

    CameraBuilder& size(glm::vec2 size) { m_size = size; return *this;}

    std::shared_ptr<Entity> build() const override;

private:
    glm::vec2 m_size;
};

} // flappy
