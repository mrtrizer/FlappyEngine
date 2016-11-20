#include "AtlasRes.h"

namespace flappy {

using namespace std;
using namespace Tools;

Rect AtlasRes::rect(const string& name) const {
    auto rectIter = m_rectMap.find(name);
    if (rectIter != m_rectMap.end())
        return rectIter->second;
    LOGE("Rect with name %s is not found in atlas.", name.data());
    return {0,0,1,1};
}

void AtlasRes::addRect(const string& name, const Rect& rect) {
    m_rectMap.emplace(name, rect);
}

} // flappy

