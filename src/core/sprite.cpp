#include "sprite.h"

namespace flappy {

using namespace std;

void Sprite::setPath(const std::string& path)
{
    auto quadPath = path;
    bool isImgPath = true; // convert image path to quad path
    for (char& c: quadPath)
        if (c == ':')
            isImgPath = false;
    if (isImgPath)
        quadPath = std::string(":") + quadPath;
    m_quad = MGR<ResManager>()->get<Quad>(quadPath);
    updateView();
}

} // flappy
