#include "FileResFactory.h"

#include <TextRes.h>

namespace flappy {

FileResFactory::FileResFactory(std::string path, std::string ext):
    m_path(path),
    m_ext(ext)
{
}

std::shared_ptr<Res> FileResFactory::load(const std::string& name, SafePtr<ResManager>) {
    return loadFile(m_path + name + m_ext);
}

bool FileResFactory::changed(const std::string& name) {
    return false;
}

} // flappy
