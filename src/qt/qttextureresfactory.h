#pragma once

#include <QImage>

#include <core/resmanager.h>

namespace flappy {

class IRes;

class QtTextureResFactory: public IResFactory {
public:
    /// @param path resource root
    QtTextureResFactory(std::string path): m_path(path) {}
    std::shared_ptr<IRes> load(const std::string& path) override;

private:
    std::string m_path;
};

} // factory
