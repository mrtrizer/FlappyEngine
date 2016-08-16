#include "atlas.h"

namespace flappy {

using namespace std;
using namespace Tools;

Atlas::Atlas(const string& dependence): m_dependence(dependence)
{

}

Rect Atlas::rect(const string& name) {
    return m_rectMap[name];
}

void Atlas::addRect(const string& name, const Rect& rect) {
    m_rectMap[name] = rect;
}

} // flappy

