#ifndef QTRESOURCEMGR_H
#define QTRESOURCEMGR_H

#include <QImage>

#include <core/resourcemgr.h>

class QtResourceMgr :public ResourceMgr
{
public:
    QtResourceMgr(QString path);
    virtual std::shared_ptr<Texture> getTexture(std::string name) const override;
private:
    QString path;
};

#endif // QTRESOURCEMGR_H
