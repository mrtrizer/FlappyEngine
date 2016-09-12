#include "atlas.h"

namespace flappy {

using namespace std;
using namespace Tools;

Atlas::Atlas(const string& dependence): m_dependence(dependence)
{

}

Rect Atlas::rect(const string& name) {
    auto rectIter = m_rectMap.find(name);
    if (rectIter != m_rectMap.end())
        return rectIter->second;
    LOGE("Rect with name %s is not found in atlas.", name.data());
    return {0,0,1,1};
}

void Atlas::addRect(const string& name, const Rect& rect) {
    m_rectMap[name] = rect;
}

} // flappy

