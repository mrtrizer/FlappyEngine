#include "quadresfactory.h"

#include <sstream>
#include "quad.h"
#include "atlas.h"

namespace flappy {

using namespace std;

void QuadResFactory::initRes(std::shared_ptr<IResHandler> quadHandler, std::shared_ptr<ResManager> resManager)  {
    auto path = quadHandler->path();
    auto splittedPath = split(path, ':');
    string atlasPath;
    string quadPath;
    if (splittedPath.size() == 2) {
        atlasPath = splittedPath[0];
        quadPath = splittedPath[1];
    } else {
        throw runtime_error("wrong path format");
    }

    if (!atlasPath.empty()) { // if atlas path, load atlas
        auto atlas = resManager->get<Atlas>(atlasPath);
        auto quad = make_shared<Quad>(atlas, quadPath, resManager);
        quadHandler->setNewResource(quad);
        quadHandler->addDependency(atlas);
    } else { // if just image path
        string textureName = quadPath;
        string defaultQuadName = "__full__";
        string defaultAtlasName = string("atlas_full__") + textureName;
        {
            auto atlas = Atlas(textureName); // create atlas dependent from image
            atlas.addRect(defaultQuadName,{0,0,1,1});
            resManager->set(defaultAtlasName, atlas);
        }
        auto defaultAtlas = resManager->get<Atlas>(defaultAtlasName);
        auto defaultQuad = make_shared<Quad>(defaultAtlas, defaultQuadName, resManager);
        quadHandler->setNewResource(defaultQuad);
        quadHandler->addDependency(defaultAtlas);
    }
}

std::vector<std::string> QuadResFactory::split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(std::move(item));
    }
    return elems;
}

} // flappy
