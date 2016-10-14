#pragma once

#include <string>

#include "res.h"

namespace flappy {

class TextFile: public Res<TextFile> {
public:
    TextFile(std::string&& text): m_text(std::move(text)) {

    }

    const std::string& text() const {
        return m_text;
    }

private:
    std::string m_text;
};

} // flappy
