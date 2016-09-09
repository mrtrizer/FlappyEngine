#include "sprite.h"
#include "viewfactory.h"

namespace flappy {

using namespace std;

void Sprite::setPath(const std::string& path)
{
    auto quadPath = path;
    bool isImgPath = true;
    for (char& c: quadPath)
        if (c == ':')
            isImgPath = false;
    if (isImgPath)
        quadPath = std::string(":") + quadPath;
    m_quad = MGR<ResourceMgr>()->get<Quad>(quadPath);
    updateView();
}

shared_ptr<View> Sprite::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

} // flappy
