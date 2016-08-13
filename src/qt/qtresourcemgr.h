#pragma once

#include <QImage>

#include <core/resourcemgr.h>

namespace flappy {

class QtResourceMgr :public ResourceMgr
{
public:
    QtResourceMgr(QString m_path);
    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const override;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const override;
private:
    QString m_path;
};

} // flappy
