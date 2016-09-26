#include "resmanager.h"

#include <sstream>

#include "texture.h"
#include "atlas.h"
#include "quad.h"

namespace flappy {

using namespace std;
using namespace Tools;

void ResManager::update(TimeDelta)
{
    for (unsigned resId = 0; resId < m_resourceMaps.size(); resId++) {
        auto factory = Tools::resizeAndGet(m_factories, resId);
        auto& map = m_resourceMaps[resId];
        for (auto i = map.begin(); i != map.end(); ) {
            if (factory != nullptr) { // load only with factory
                if (i->second->loading()) {
                    auto res = factory->load(i->second->path());
                    i->second->setNewResource(res);
                    factory->initHandler(i->second, res, shared_from_this());
                }
            }
            i->second->update();
            if (i->second.use_count() == 1)
                i = map.erase(i);
            else
                i++;
        }
    }
}

} // flappy
