#pragma once

#include <fstream>
#include <sstream>

#include <core/iresfactory.h>

namespace flappy {

class TextFileFactoryDesktop: public IResFactory {
public:
    TextFileFactoryDesktop(std::string path): m_path(path) {}
    std::shared_ptr<IRes> load(const std::string& path) override;

private:
    std::string m_path;
};

}
