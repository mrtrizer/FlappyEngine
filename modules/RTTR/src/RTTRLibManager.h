#pragma once

#include <Manager.h>
#include <rttr/library.h>

namespace flappy {

class RTTRLibrary {
public:
    explicit RTTRLibrary(const std::string& filePath);

    rttr::type findType(const std::string& name);
    rttr::variant toVariant(std::shared_ptr<ComponentBase> component);
    std::shared_ptr<ComponentBase> createComponent(std::string name);

private:
    rttr::library m_rttrLibrary;
};

class RTTRLibManager : public Manager<RTTRLibManager> {
public:
    RTTRLibrary& getLibrary(const std::string& filePath);
private:
    std::unordered_map<std::string, std::unique_ptr<RTTRLibrary>> m_libraries;
};

} // flappy
