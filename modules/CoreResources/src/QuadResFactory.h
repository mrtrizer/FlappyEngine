#pragma once

#include <memory>
#include <vector>

#include <IResFactory.h>

namespace flappy {

class ResManager;

class QuadResFactory: public IResFactory {
public:
    /// @brief Loads sprite by path in atlas or file name.
    /// @details Always loads atlas synchronous.
    /// There are two name formats avaliable:
    /// Path in atlas: "atlas_name:sprite_name"
    /// Image file name: "file_name"
    std::shared_ptr<Res> load(const std::string& name, SafePtr<Entity>) override;

private:
    std::vector<std::string> split(const std::string &s, char delimiter);
};

} // flappy
