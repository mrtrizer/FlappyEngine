#pragma once

#include <Builder.h>

namespace flappy {

class CameraBuilder : public Builder
{
public:
    CameraBuilder& size(glm::vec2 size) { m_size = size; return *this;}

    void build(std::shared_ptr<Entity> entity) const override;

private:
    glm::vec2 m_size;
};

} // flappy
