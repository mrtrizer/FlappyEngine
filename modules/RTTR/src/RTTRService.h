#pragma once

#include <rttr/library.h>
#include <rttr/type.h>
#include <rttr/variant.h>

#include <ComponentBase.h>

namespace flappy {

class RTTRService {
public:
    RTTRService(const RTTRService&) = delete;
    RTTRService& operator=(const RTTRService&) = delete;
    RTTRService(RTTRService&&) = delete;
    RTTRService& operator=(RTTRService&&) = delete;

    static RTTRService& instance();

    void loadLibrary(const std::string& filePath);

    rttr::variant toVariant(const std::shared_ptr<ComponentBase>& component);
    std::shared_ptr<ComponentBase> createComponent(TypeId<ComponentBase> type);
    rttr::type toRTTRPointerType(TypeId<ComponentBase> type);
    rttr::type toRTTRType(TypeId<ComponentBase> type);

private:
    RTTRService() = default;
    rttr::type findType(const std::string& name);

    std::unique_ptr<rttr::library> m_rttrLibrary;
};

} // flappy
