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
    for (unsigned j = 0; j < m_resourceMap.size(); j++) {
        auto factory = m_factories[j];
        if (factory == nullptr)
            continue;
        auto map = m_resourceMap[j];
        for (auto i = map.begin(); i != map.end(); ) {
            i->second->update();
            if (i->second->markedReload()) {
                auto res = factory->load(i->second->path());
                i->second->setNewResource(res);
                factory->initHandler(i->second, res, shared_from_this());
            }
            if (i->second.use_count() == 1)
                i = map.erase(i);
            else
                i++;
        }
    }
}

} // flappy
