#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>

namespace flappy {

class ResManager;

class QuadResFactory: public ResFactory {
public:
    std::shared_ptr<Res> load(const std::string& name, std::shared_ptr<ResManager>) override;

private:
    std::vector<std::string> split(const std::string &s, char delimiter);
};

} // flappy
