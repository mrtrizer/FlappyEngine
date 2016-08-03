#pragma once

#include <QImage>

#include <core/resourcemgr.h>

namespace flappy {

class QtResourceMgr :public ResourceMgr
{
public:
    QtResourceMgr(QString m_path);
    virtual shared_ptr<Texture> getTexture(const string& name) const override;
private:
    QString m_path;
};

} // flappy
