#include "qtresourcemgr.h"
#include <gl/gltexture.h>

namespace flappy {

QtResourceMgr::QtResourceMgr(QString path):
    m_path(path) {

}

shared_ptr<Texture> QtResourceMgr::getTexture(const string& name) const {
    QImage image(m_path + QString::fromStdString(name) + QString(".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = make_shared<GLTexture>(bits, image.width(), image.height());
    return result;
}

} // flappy
