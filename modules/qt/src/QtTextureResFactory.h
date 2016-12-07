#pragma once

#include <QImage>

#include <FileResFactory.h>

namespace flappy {

class QtTextureResFactory: public FileResFactory {
public:
    /// @param path resource root
    QtTextureResFactory(std::string path, std::shared_ptr<IFileMonitor> fileMonitor);
    std::shared_ptr<Res> create(const std::string&, std::shared_ptr<ResManager>) override;
    std::shared_ptr<Res> loadFile(const std::string& path) override;
};

} // factory
