#include "StdTextFileResFactory.h"

#include <TextRes.h>
#include <IFileMonitor.h>

namespace flappy {

StdTextFileResFactory::StdTextFileResFactory(std::string path, std::shared_ptr<IFileMonitor> fileMonitor):
    FileResFactory(path, "", fileMonitor)
{}

std::shared_ptr<Res> StdTextFileResFactory::loadFile(const std::string& path) {
    std::ifstream fStream(path);
    std::stringstream sStream;
    sStream << fStream.rdbuf();
    fStream.close();
    return std::make_shared<TextRes>(sStream.str());
}

std::shared_ptr<Res> StdTextFileResFactory::create(const std::string&, std::shared_ptr<ResManager>) {
    return std::make_shared<TextRes>("");
}

}
