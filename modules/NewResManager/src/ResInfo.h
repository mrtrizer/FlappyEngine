#pragma once

#include <string>
#include <unordered_map>

namespace flappy {

/// @addtogroup ResManager
/// @{

struct FileInfo {
    std::string name;
    std::string path;
    std::string type;
};

struct ResMeta {
    std::unordered_map<std::string, std::string> data;
};

/// @}

} // flappy
