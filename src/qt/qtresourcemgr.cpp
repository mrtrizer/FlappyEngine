#include "qtresourcemgr.h"
#include "gl/gltexture.h"

QtResourceMgr::QtResourceMgr(QString path):
    m_path(path) {

}

std::shared_ptr<Texture> QtResourceMgr::getTexture(const std::string &name) const {
    QImage image(m_path + QString::fromStdString(name) + QString(".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = std::make_shared<GLTexture>(bits, image.width(), image.height());
    return result;
}
