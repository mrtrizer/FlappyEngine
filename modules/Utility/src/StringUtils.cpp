#include "StringUtils.hpp"

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

}

