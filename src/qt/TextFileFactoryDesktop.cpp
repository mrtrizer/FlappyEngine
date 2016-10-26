#include "TextFileFactoryDesktop.h"

#include <core/textfile.h>

namespace flappy {

std::shared_ptr<IRes> TextFileFactoryDesktop::load(const std::string& path) {
    std::ifstream fStream(m_path + path);
    std::stringstream sStream;
    sStream << fStream.rdbuf();
    fStream.close();
    return std::make_shared<TextFile>(sStream.str());
}

}
