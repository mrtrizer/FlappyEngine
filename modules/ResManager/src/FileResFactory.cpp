#include "FileResFactory.h"

#include <TextRes.h>
#include <Entity.h>
#include <IFileLoadManager.h>

namespace flappy {

FileResFactory::FileResFactory(std::string path, std::string ext):
    m_path(path),
    m_ext(ext)
{}

std::shared_ptr<Res> FileResFactory::load(const ResInfo& resInfo, SafePtr<Entity> resManagerEntity) {
    auto fileLoadManager = resManagerEntity->manager<IFileLoadManager>();
    return std::make_shared<TextRes>(fileLoadManager->loadTextFile(resInfo.path));
}

std::shared_ptr<Res> FileResFactory::create(const std::string& name, SafePtr<Entity> resManagerEntity) {
    return std::make_shared<TextRes>("");
}

} // flappy
