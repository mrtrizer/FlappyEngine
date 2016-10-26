#pragma once

#include <QImage>

#include <core/iresfactory.h>

namespace flappy {

class IRes;

class TextureFactoryQt: public IResFactory {
public:
    /// @param path resource root
    TextureFactoryQt(std::string path): m_path(path) {}
    std::shared_ptr<IRes> load(const std::string& path) override;

private:
    std::string m_path;
};

} // factory
