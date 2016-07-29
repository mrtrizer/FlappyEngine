#ifndef QTRESOURCEMGR_H
#define QTRESOURCEMGR_H

#include <QImage>

#include <core/resourcemgr.h>

class QtResourceMgr :public ResourceMgr
{
public:
    QtResourceMgr(QString m_path);
    virtual std::shared_ptr<Texture> getTexture(const std::string& name) const override;
private:
    QString m_path;
};

#endif // QTRESOURCEMGR_H
