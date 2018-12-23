#pragma once

#include <string>
#include <vector>

namespace StringUtils {
    std::string joinPath(std::vector<std::string> pathList);
    
    std::vector<std::string> split(const std::string &s, char delimiter);
}
