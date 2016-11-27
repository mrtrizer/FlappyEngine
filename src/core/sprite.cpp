#include "sprite.h"

#include <managers/ResManager/ResManager.h>
#include <resources/QuadRes.h>

namespace flappy {

using namespace std;

void Sprite::setPath(const std::string& path)
{
    m_quad = MGR<ResManager>()->getRes<QuadRes>(path);
    updateView();
}

} // flappy
