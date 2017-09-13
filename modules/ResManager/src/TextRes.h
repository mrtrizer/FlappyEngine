#pragma once

#include <string>

#include <Res.h>

namespace flappy {

class TextRes: public Res {
public:
    TextRes(std::string&& text): m_text(std::move(text))
    {}

    const std::string& text() const
    {
        return m_text;
    }

private:
    std::string m_text;
};

} // flappy
