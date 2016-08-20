#pragma once

#include <QImage>

#include <core/iresourceloader.h>

namespace flappy {

class QtResourceLoader :public IResourceLoader
{
public:
    QtResourceLoader(QString m_path);
    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const override;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const override;
private:
    QString m_path;
};

} // flappy
