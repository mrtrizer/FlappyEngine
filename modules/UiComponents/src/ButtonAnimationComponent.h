#pragma once

namespace flappy {

class ButtonAnimationComponent
{
public:
    ButtonAnimationComponent();
    virtual ~ButtonAnimationComponent() = default;
    ButtonAnimationComponent(const ButtonAnimationComponent&) = delete;
    ButtonAnimationComponent& operator=(const ButtonAnimationComponent&) & = delete;
    ButtonAnimationComponent(ButtonAnimationComponent&&) = delete;
    ButtonAnimationComponent& operator=(ButtonAnimationComponent&&) & = delete;
};

} // flappy
