#include "glviewfactoryqt.h"
#include "gl/gltexture.h"

GLViewFactoryQt::GLViewFactoryQt(QString path):
    path(path) {

}

std::shared_ptr<GLTexture> GLViewFactoryQt::getGLTexture(std::string name) const {
    QImage image(path + QString::fromStdString(name) + QString(".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = std::make_shared<GLTexture>(bits, image.width(), image.height());
    return result;
}
