#pragma once

#include <memory>

#include "iresfactory.h"

namespace flappy {

class IRes;

class AtlasResFactory: public IResFactory {
public:
    std::shared_ptr<IRes> load(const std::string&) override;
    void updateHandler(std::shared_ptr<IResHandler>, std::shared_ptr<IRes>, std::shared_ptr<ResManager>) override;
};

} // flappy
