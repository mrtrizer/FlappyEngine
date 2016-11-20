#include "QtTextureResFactory.h"

#include <memory>

#include <QImage>

#include <gl/resources/GLTextureRes.h>

namespace flappy {

using namespace std;

QtTextureResFactory::QtTextureResFactory(std::string path, std::shared_ptr<IFileMonitor> fileMonitor):
    FileResFactory(path, ".png", fileMonitor)
{}

std::shared_ptr<Res> QtTextureResFactory::create(const std::string&, std::shared_ptr<ResManager>) {
    const unsigned char bitmapData[] = {0xff,0x55,0xff,0x55};
    auto texture = make_shared<GLTexture>((const char*)bitmapData, 1, 1);
    return texture;
}

std::shared_ptr<Res> QtTextureResFactory::loadFile(const std::string& path) {
    QImage image(QString::fromStdString(path));
    const char* bitmapData = reinterpret_cast<char *>(image.bits());
    auto texture = make_shared<GLTexture>(bitmapData, image.width(), image.height());
    return texture;
}

} // flappy
