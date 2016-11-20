#pragma once

#include <fstream>
#include <sstream>

#include <managers/ResManager/FileResFactory.h>

namespace flappy {

class IFileMonitor;

class StdTextFileResFactory: public FileResFactory {
public:
    StdTextFileResFactory(std::string path, std::shared_ptr<IFileMonitor>);
    std::shared_ptr<Res> loadFile(const std::string& path) override;
    std::shared_ptr<Res> create(const std::string&, std::shared_ptr<ResManager>) override;
};

}
