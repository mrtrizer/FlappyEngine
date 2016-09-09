#pragma once

#include <unordered_map>

#include "tools.h"

namespace flappy {

class Atlas
{
public:
    Atlas(const std::string& dependence);
    /// Can't be const because m_rectMap[] is not const
    Tools::Rect rect(const std::string& name);
    void addRect(const std::string& name, const Tools::Rect &rect);
    std::string dependence() const {return m_dependence; }

private:
    std::unordered_map<std::string, Tools::Rect> m_rectMap;
    std::string m_dependence;
};

} // flappy
