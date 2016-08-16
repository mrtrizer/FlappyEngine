#include "qtresourcemgr.h"
#include <gl/gltexture.h>
#include <core/atlas.h>

namespace flappy {

using namespace std;

QtResourceMgr::QtResourceMgr(QString path):
    m_path(path) {

}

unique_ptr<Texture> QtResourceMgr::getTexture(const string& name) const {
    QImage image(m_path + QString::fromStdString(name) + QString(".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = unique_ptr<GLTexture>(new GLTexture(bits, image.width(), image.height()));
    return move(result);
}

unique_ptr<Atlas> QtResourceMgr::getAtlas(const string&) const {
    return move(unique_ptr<Atlas>(new Atlas("")));
}


} // flappy
