#pragma once

#include <Builder.h>

namespace flappy {

class MenuLayoutBuilder: public Builder
{
public:
    using Builder::Builder;

    struct StartButtonPressed {};

    std::shared_ptr<Entity> build() const override;
};

} // flappy
