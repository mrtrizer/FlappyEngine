#pragma once

#include <Builder.h>

namespace flappy {

class SpriteBuilder : public Builder
{
public:
    SpriteBuilder& spritePath(std::string path) { m_spritePath = path; return *this; }

    void build(std::shared_ptr<Entity> entity) const override;

private:
    std::string m_spritePath;
};

} // flappy
