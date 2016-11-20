#pragma once

#include <unordered_map>

#include <core/tools.h>
#include <managers/ResManager/Res.h>

namespace flappy {

class AtlasRes: public Res
{
public:
    AtlasRes() = default;
    Tools::Rect rect(const std::string& name) const;
    void addRect(const std::string& name, const Tools::Rect &rect);
    std::list<std::shared_ptr<Res>> dependencyList() const override { return {}; }

private:
    std::unordered_map<std::string, Tools::Rect> m_rectMap;
};

} // flappy
