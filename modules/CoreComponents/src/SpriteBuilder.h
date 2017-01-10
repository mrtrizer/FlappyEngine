#pragma once

#include <Builder.h>

namespace flappy {

class SpriteBuilder : public Builder
{
public:
    using Builder::Builder;

    SpriteBuilder& spritePath(std::string path) { m_spritePath = path; return *this; }

    std::shared_ptr<Entity> build() const override;

private:
    std::string m_spritePath;
};

} // flappy
