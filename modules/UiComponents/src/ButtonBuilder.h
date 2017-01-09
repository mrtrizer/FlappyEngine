#pragma once

#include <Builder.h>

namespace flappy {

class ButtonBuilder: public Builder
{
public:
    ButtonBuilder& idlePath(std::string path) { m_idlePath = path; return *this; }
    ButtonBuilder& activatedPath(std::string path) { m_activatedPath = path; return *this; }
    ButtonBuilder& pressedPath(std::string path) { m_pressedPath = path; return *this; }
    ButtonBuilder& onClick(std::function<void()> handler) { m_onClickHandler = handler; return *this; }

    void build(std::shared_ptr<Entity>) const override;

private:
    std::string m_idlePath;
    std::string m_activatedPath;
    std::string m_pressedPath;
    std::function<void()> m_onClickHandler;
};

} // flappy
