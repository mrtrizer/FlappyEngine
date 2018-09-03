#pragma once

#include "TypeId.hpp"
#include "Function.hpp"
#include "Type.hpp"
#include "Reflection.hpp"

namespace flappy {

class BasicTypesReflection {
public:
    static BasicTypesReflection& instance() {
        static BasicTypesReflection standardTypesReflection;
        return standardTypesReflection;
    }
    BasicTypesReflection() : m_reflection(std::make_shared<Reflection>()) {
        m_reflection->registerType<std::string>(
                    "std::string",
                    ConstructorRef<const char*>(),
                    ConstructorRef<std::string>(),
                    ConstructorRef<size_t, char>(),
                    MethodRef<std::string, unsigned long>("capacity", [](std::string& obj){ return obj.capacity(); } )
                );
    }

    const std::shared_ptr<Reflection>& reflection() const { return m_reflection; }

private:
    std::shared_ptr<Reflection> m_reflection;
};

} // flappy
