#include "RTTRLibManager.h"
#include <rttr/library.h>

namespace flappy {

RTTRLibrary::RTTRLibrary(const std::string& filePath)
    : m_rttrLibrary(filePath)
{
    if (!m_rttrLibrary.load())
        throw std::runtime_error("Can't load library.");
}

rttr::type RTTRLibrary::findType(const std::string& name) {
    auto types = m_rttrLibrary.get_types();
    auto foundIter = std::find_if(types.begin(), types.end(), [name](auto item){
        return item.get_name() == name;
    });
    if (foundIter == types.end())
        throw std::runtime_error("Can't find RTTR type " + name);
    return *foundIter;
}

rttr::variant RTTRLibrary::toVariant(std::shared_ptr<ComponentBase> component) {
    auto componentId = component->componentId();
    auto componentType = componentId.name().substr(componentId.name().find_last_of(':') + 1) + "*";
    auto componentVariant = findType(componentType).create_variant(component.get());
    return componentVariant;
}

std::shared_ptr<ComponentBase> RTTRLibrary::createComponent(std::string name) {
    return findType(name).create().get_value<std::shared_ptr<ComponentBase>>();
}

RTTRLibrary& RTTRLibManager::getLibrary(const std::string& filePath) {
    auto iter = m_libraries.find(filePath);
    return iter == m_libraries.end() ?
            *m_libraries.emplace(filePath, std::make_unique<RTTRLibrary>(filePath)).first->second : *iter->second;
}

} // flappy
