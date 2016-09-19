#include "reshandler.h"

#include "resmanager.h"
#include "texture.h"
#include "atlas.h"

namespace flappy {

template <>
void ResHandler<Atlas>::procNewResource(std::shared_ptr<ResManager> resManager)
{
    m_updated = false;
    m_resource = std::move(m_newResource);
    addDependency(resManager->get<Texture>(m_resource->dependence()));
}

} // flappy
