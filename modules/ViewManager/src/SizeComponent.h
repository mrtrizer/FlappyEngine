#pragma once

#include <glm/vec3.hpp>
#include <Component.h>

namespace flappy {

class SizeComponent : public Component
{
public:
    SizeComponent();
    virtual ~SizeComponent() = default;
    glm::vec3 size() { return m_size; }
    void setSize(glm::vec3 size) { m_size = size; }

private:
    glm::vec3 m_size;
};

} // flappy
