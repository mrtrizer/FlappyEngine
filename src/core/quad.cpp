#include "quad.h"

#include <core/texture.h>
#include <core/atlas.h>
#include <core/resourcemgr.h>

namespace flappy {

using namespace std;
using namespace Tools;

Rect Quad::rect() {
    return m_atlas->resource().rect(m_quadName);
}

std::shared_ptr<ResourceHandler<Texture>> Quad::texture() {
    if (!m_atlas->ready())
        return nullptr;
    return m_resourceMgr->get<Texture>(m_atlas->resource().dependence());
}

} // flappy
