#include "viewfactoryqt.h"
#include "gl/gltexture.h"

ViewFactoryQt::ViewFactoryQt(QString path):
    path(path) {

}

std::shared_ptr<Texture> ViewFactoryQt::getTexture(std::string name) const {
    QImage image(path + QString::fromStdString(name) + QString(".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = std::make_shared<GLTexture>(bits, image.width(), image.height());
    return result;
}
