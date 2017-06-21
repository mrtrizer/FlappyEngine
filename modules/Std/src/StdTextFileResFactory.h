#pragma once

#include <fstream>
#include <sstream>

#include <FileResFactory.h>

namespace flappy {

class IFileMonitor;

class StdTextFileResFactory: public FileResFactory {
public:
    StdTextFileResFactory(std::string path, std::shared_ptr<IFileMonitor>);
    std::shared_ptr<Res> loadFile(const std::string& path) override;
    std::shared_ptr<Res> create(const std::string&, SafePtr<ResManager>) override;
};

}
