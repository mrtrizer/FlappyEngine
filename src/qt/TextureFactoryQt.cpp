#include "TextureFactoryQt.h"

#include <memory>

#include <QImage>

#include <core/res.h>
#include <gl/gltexture.h>

namespace flappy {

using namespace std;

std::shared_ptr<IRes> TextureFactoryQt::load(const std::string& path) {
    QImage image(QString::fromStdString(m_path + path + ".png"));
    const char * bits = reinterpret_cast<char *>(image.bits());
    auto result = make_shared<GLTexture>(bits, image.width(), image.height());
    return std::dynamic_pointer_cast<IRes>(result);
}

} // flappy
