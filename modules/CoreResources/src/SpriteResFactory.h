#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>

#include "SpriteRes.h"

namespace flappy {

class SpriteResFactory: public ResFactory<SpriteRes> {
public:
    SpriteResFactory();

    /// @brief Loads sprite by path in atlas or file name.
    /// @details Always loads atlas synchronous.
    /// There are two name formats avaliable:
    /// Path in atlas: "atlas_name:sprite_name"
    /// Image file name: "file_name"
    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
};

} // flappy
