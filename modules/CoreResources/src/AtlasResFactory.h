#pragma once

#include <string>

#include <ResFactory.h>
#include <AtlasRes.h>

namespace flappy {

class AtlasResFactory :public ResFactory<AtlasRes>
{
public:
    AtlasResFactory();
    std::shared_ptr<ResBase> load(const std::string&name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string&) final;
};

} // flappy
