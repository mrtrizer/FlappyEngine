#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>

#include "FontRes.h"

namespace flappy {

class FontResFactory: public ResFactory<FontRes> {
public:
    FontResFactory();

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
};

} // flappy
