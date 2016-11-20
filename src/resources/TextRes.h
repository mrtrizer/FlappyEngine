#pragma once

#include <string>

#include <managers/ResManager/Res.h>

namespace flappy {

class TextRes: public Res {
public:
    TextRes(std::string&& text): m_text(std::move(text))
    {}

    const std::string& text() const
    {
        return m_text;
    }

    std::list<std::shared_ptr<Res>> dependencyList() const override { return {}; }

private:
    std::string m_text;
};

} // flappy
