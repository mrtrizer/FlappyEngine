#pragma once

#include <Builder.h>

namespace flappy {

class MenuLayoutBuilder: public Builder
{
public:
    struct StartButtonPressed {};

    void build(std::shared_ptr<Entity>) const override;
};

} // flappy
