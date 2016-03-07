#ifndef GLVIEWFACTORYQT_H
#define GLVIEWFACTORYQT_H

#include <QImage>

#include <gl/glviewfactory.h>

class GLViewFactoryQt :public GLViewFactory
{
public:
    GLViewFactoryQt(QString path);
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string name) const override;
private:
    QString path;
};

#endif // GLVIEWFACTORYQT_H
