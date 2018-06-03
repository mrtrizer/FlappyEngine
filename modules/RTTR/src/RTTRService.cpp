#include "RTTRService.h"

namespace flappy {

RTTRService& RTTRService::instance() {
    static RTTRService intance;
    return intance;
}

void RTTRService::loadLibrary(const std::string& filePath) {
    if (m_rttrLibrary && m_rttrLibrary->is_loaded()) {
        if (!m_rttrLibrary->unload())
            throw std::runtime_error("Can't unload library.");
    }
    m_rttrLibrary = std::make_unique<rttr::library>(filePath);
    if (!m_rttrLibrary->load()) {
        throw std::runtime_error("Can't load library %s.");
    }
}

rttr::type RTTRService::findType(const std::string& name) {
    auto types = rttr::type::get_types();
    auto foundIter = std::find_if(types.begin(), types.end(), [name](auto item){
        return item.get_name() == name;
    });
    if (foundIter == types.end())
        throw std::runtime_error("Can't find RTTR type " + name);
    return *foundIter;
}

rttr::variant RTTRService::toVariant(const std::shared_ptr<ComponentBase>& component) {
    auto componentId = component->componentId();
    auto componentVariant = toRTTRPointerType(componentId).create_variant(component.get());
    return componentVariant;
}

std::shared_ptr<ComponentBase> RTTRService::createComponent(TypeId<ComponentBase> type) {
    try {
        auto rttrType = toRTTRType(type);
        if (!rttrType.is_valid())
            throw new std::runtime_error("Type is invalid");
        auto rttrVariant = rttrType.create();
        if (!rttrVariant.is_valid())
            throw new std::runtime_error("Variant is invalid");
        auto instance = rttrVariant.get_value<std::shared_ptr<ComponentBase>>();
        if (instance == nullptr)
            throw new std::runtime_error("Invalid cast to ComponentBase");
        return instance;
    } catch (const std::exception& e) {
        LOGE("Can't create component of type %s. %s", type.name().c_str(), e.what());
        return nullptr;
    }
}

rttr::type RTTRService::toRTTRPointerType(TypeId<ComponentBase> type) {
    auto componentType = type.name().substr(type.name().find_last_of(':') + 1) + "*";
    return findType(componentType);
}

rttr::type RTTRService::toRTTRType(TypeId<ComponentBase> type) {
    auto componentType = type.name().substr(type.name().find_last_of(':') + 1);
    return findType(componentType);
}

} // flappy
