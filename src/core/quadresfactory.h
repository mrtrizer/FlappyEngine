#pragma once

#include <memory>

#include "resmanager.h"

namespace flappy {

class QuadResFactory: public IResFactory {
public:
    void initRes(std::shared_ptr<IResHandler> quadHandler, std::shared_ptr<ResManager> resManager) override;
private:
    std::vector<std::string> split(const std::string &s, char delim);
};


} // flappy
