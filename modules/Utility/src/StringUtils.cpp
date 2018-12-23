#include "StringUtils.hpp"

#include <sstream>

namespace StringUtils {

    std::string joinPath(std::vector<std::string> pathList) {
        if (pathList.size() < 1)
            return "";
        std::string outPath = pathList[0];
        for (int i = 1; i < pathList.size(); i++) {
            outPath += "/" + pathList[i];
        }
        return outPath;
    }

    std::vector<std::string> split(const std::string &s, char delimiter) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delimiter)) {
            elems.push_back(std::move(item));
        }
        return elems;
    }
    
}

