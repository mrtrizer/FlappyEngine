#include "FileResFactory.h"

#include <IFileMonitor.h>

namespace flappy {

FileResFactory::FileResFactory(std::string path, std::string ext, std::shared_ptr<IFileMonitor> fileMonitor):
    m_path(path),
    m_ext(ext),
    m_fileMonitor(fileMonitor)
{
}

std::shared_ptr<Res> FileResFactory::load(const std::string& name, std::shared_ptr<ResManager>) {
    if (m_fileMonitor != nullptr)
        m_fileMonitor->registerFile(m_path + name + m_ext);
    return loadFile(m_path + name + m_ext);
}

bool FileResFactory::changed(const std::string& name) {
    if (m_fileMonitor != nullptr)
        return m_fileMonitor->changed(m_path + name + m_ext);
    return false;
}

} // flappy
