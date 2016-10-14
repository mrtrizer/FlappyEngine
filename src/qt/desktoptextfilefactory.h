#pragma once

#include <fstream>
#include <sstream>

#include <core/resmanager.h>
#include <core/textfile.h>

namespace flappy {

class DesktopTextFileFactory: public IResFactory {
public:
    DesktopTextFileFactory(std::string path): m_path(path) {}
    std::shared_ptr<IRes> load(const std::string& path) override {
        std::ifstream fStream(m_path + path);
        std::stringstream sStream;
        sStream << fStream.rdbuf();
        fStream.close();
        return std::make_shared<TextFile>(sStream.str());
    }

private:
    std::string m_path;
};

}
