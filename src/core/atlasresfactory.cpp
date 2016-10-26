#include "atlasresfactory.h"

#include "res.h"
#include "atlas.h"
#include "texture.h"
#include "resmanager.h"

namespace flappy {

std::shared_ptr<IRes> AtlasResFactory::load(const std::string&) {
    auto atlas = std::make_shared<Atlas>("");
    return std::dynamic_pointer_cast<IRes>(atlas);
}

void AtlasResFactory::updateHandler(std::shared_ptr<IResHandler> handler, std::shared_ptr<IRes> res, std::shared_ptr<ResManager> resManager) {
    auto atlas = std::static_pointer_cast<Atlas>(res);
#warning Clear existing dependencies before adding new ones.
    handler->addDependency(resManager->get<Texture>(atlas->dependence()));
}

} // flappy
